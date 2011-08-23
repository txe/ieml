//===================================================================
//
// logger.h - logger
//
// Author: Димчанский
// Thx to: WolfHound, remark, other rsdn-members
// Created: Jan 2007
//
//===================================================================



#ifndef __LOGGER_H
#define __LOGGER_H

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <sys/types.h>
#include <sys/timeb.h>

#include <windows.h>

namespace logger
{
	enum LogType
	{
		None    = -3,
		Error   = -2,
		Warning = -1,
		Debug   = 0
	};

	/////////////////////////////////////////////////////////////////////
	// Stream Log
	/////////////////////////////////////////////////////////////////////

	template <typename L>
	class StreamLog
	{
	public:
		typedef L LogType;

		StreamLog(L& log, int logLevel = Debug)
			: m_logLevel(logLevel)
			, m_log(log)
			, m_strBuffer()
		{
		}

		StreamLog(StreamLog<L>& rhs)
			: m_logLevel(rhs.m_logLevel)
			, m_log(rhs.m_log)
			// при копировании создаём свой поток, т.к. StreamLog может скопироваться
			// только до первой операции << 
			, m_strBuffer() 
		{
			// оригинальный делаем плохим, чтобы он в деструкторе ничего не сделал
			rhs.setBufferBad();
		}

		~StreamLog()
		{
			if(!m_strBuffer.bad())
			{
				m_log.write(m_logLevel, m_strBuffer.str());
			}
		};

		template <typename T>
		std::ostream& operator<<(const T& val)
		{
			m_strBuffer << val;
			return m_strBuffer;
		}

	private:
		int m_logLevel;
		L& m_log;
		std::ostringstream m_strBuffer; // буфер, куда будут складываться все записи по оператору "<<" до ";"

		StreamLog<L>& operator=(const StreamLog<L>&);

		inline void setBufferBad() { m_strBuffer.setstate(std::ios_base::badbit); }
	};

	/////////////////////////////////////////////////////////////////////
	// Log template
	/////////////////////////////////////////////////////////////////////

	template <typename LogWriter, typename LogLock>
	class Log
	{
	public:
		Log()
			: m_logLevel(None)
		{}

		void write(int logLevel, const std::string& logMessage)
		{
			m_guard.Lock();

			if(canLog_(logLevel))
			{
				m_log.write(logLevel, logMessage);
			}

			m_guard.Unlock();
		}

		void setLogLevel(int logLevel)
		{
			m_guard.Lock();
			m_logLevel = logLevel;
			m_guard.Unlock();
		}

		int getLogLevel()
		{
			m_guard.Lock();
			int retval = m_logLevel;
			m_guard.Unlock();
			return retval;
		}

		bool canLog(int logLevel)
		{
			m_guard.Lock();
			bool retval = canLog_(logLevel);
			m_guard.Unlock();
			return retval;
		}

		void setLogPrefix(const std::string& prefix)
		{
			m_guard.Lock();
			m_log.setPrefix(prefix);
			m_guard.Unlock();
		}

	private:
		int m_logLevel;
		LogWriter m_log;
		LogLock m_guard;

		Log(const Log<LogWriter, LogLock>&);
		Log<LogWriter, LogLock>& operator=(const Log<LogWriter, LogLock>&);

		inline bool canLog_(int logLevel)
		{
			return (logLevel <= m_logLevel) && m_log.canWrite();
		}
	};

	/////////////////////////////////////////////////////////////////////
	// Standard Writers
	/////////////////////////////////////////////////////////////////////

	// Этот LogWriter просто выводит на std::cout всё.
	class COUTWriter
	{
	public:
		COUTWriter() {}

		inline void write(int logLevel, const std::string& logMessage)
		{
			std::cout << (logLevel < Debug ? (logLevel == -2 ? "ERROR" : "WARNING") : "Debug")
				<< ": " << logMessage << std::endl;
		}

		inline bool canWrite() const { return true; }

		inline void setPrefix(const std::string& /*prefix*/) {}

	private:
		COUTWriter(const COUTWriter&);
		COUTWriter& operator=(const COUTWriter&);
	};

	// Этот LogWriter пишет всё в файл.
	// Дополнительно выводится время, thread id
	class FileWriter
	{
	public:
		FileWriter()
			: m_file()
			, m_prefix("log")
			, m_canWrite(true)
		{
		}

		~FileWriter()
		{
			closeFile();
		}

		inline void write(int logLevel, const std::string& logMessage)
		{
			__timeb64 tstruct;
			_ftime64(&tstruct);
			tm lTime = *_localtime64(&tstruct.time);

			if(!m_file.is_open())
			{
				if(!openFile())
					return;
			}
			else if(lTime.tm_year != m_timeStartTM.tm_year ||
				lTime.tm_mon  != m_timeStartTM.tm_mon  || 
				lTime.tm_mday != m_timeStartTM.tm_mday)
			{
				// при изменении хотя бы дня делаем новый лог-файл
				closeFile();
				if(!openFile())
					return;
			}

			m_file << std::setiosflags(std::ios::right) << std::setfill('0')
				<< std::setw(2) << lTime.tm_hour
				<< ":"
				<< std::setw(2) << lTime.tm_min
				<< ":"
				<< std::setw(2) << lTime.tm_sec
				<< "_"
				<< std::setw(3) << tstruct.millitm
				<< "_"
				<< std::hex
				<< std::setw(8) << ::GetCurrentThreadId()
				<< std::dec
				<< ": "
				<< (logLevel < Debug ? (logLevel == -2 ? "ERROR" : "WARNING") : "Debug");
			if (logLevel > Debug)
				m_file << "(" << logLevel << ")";
			m_file << ": " << logMessage << std::endl << std::flush;
			OutputDebugStringA(logMessage.c_str());
			if(!m_file)
			{
				m_canWrite = false;
			}
		}

		inline bool canWrite() const { return m_canWrite; }

		inline void setPrefix(const std::string& prefix)
		{
			closeFile();
			m_prefix = prefix;
		}

	private:
		std::ofstream m_file;
		std::string m_prefix;
		bool m_canWrite;
		tm m_timeStartTM;

		FileWriter(const FileWriter&);
		FileWriter& operator=(const FileWriter&);

		bool openFile()
		{
			__timeb64 tstruct;
			_ftime64(&tstruct);
			tm lTime = *_localtime64(&tstruct.time);

			std::ostringstream os;

			os << m_prefix
				<< "_"
				<< (1900+lTime.tm_year)
				<< std::setiosflags(std::ios::right) << std::setfill('0')
				<< std::setw(2) << (1+lTime.tm_mon)
				<< std::setw(2) << lTime.tm_mday
		//		<< "_"
		//		<< std::setw(2) << lTime.tm_hour
		//		<< std::setw(2) << lTime.tm_min
		//		<< std::setw(2) << lTime.tm_sec
		//		<< "_"
		//		<< std::setw(3) << tstruct.millitm
				<< ".txt";

			m_file.open(os.str().c_str(), std::ios_base::out | std::ios_base::trunc);

			if(m_file)
			{
				m_timeStartTM = *_localtime64(&tstruct.time);
				return true;
			}
			else
			{
				m_canWrite = false;
			}

			return false;
		}

		inline void closeFile()
		{
			if(m_file.is_open())
			{
				m_file.close();
			}
		}

	};

	/////////////////////////////////////////////////////////////////////
	// Standard Locks
	/////////////////////////////////////////////////////////////////////

	// блокировка с использованием критических секций
	class CriticalSectionLock
	{
	public:
		CriticalSectionLock()  { ::InitializeCriticalSection(&mtx_); }
		~CriticalSectionLock() { ::DeleteCriticalSection(&mtx_);     }
		inline void Lock()     { ::EnterCriticalSection(&mtx_);      }
		inline void Unlock()   { ::LeaveCriticalSection(&mtx_);      }
	private:
		CRITICAL_SECTION mtx_;
		CriticalSectionLock(const CriticalSectionLock&);
		CriticalSectionLock& operator=(const CriticalSectionLock&);
	};

	// без блокировки
	class SingleThreadLock
	{
	public:
		SingleThreadLock() {}
		inline void Lock() {}
		inline void Unlock() {}
	private:
		SingleThreadLock(const SingleThreadLock&);
		SingleThreadLock& operator=(const SingleThreadLock&);
	};

	// просто вывод на консоль сообщений при блокировке/разблокировке
	class SingleTraceLock
	{
	public:
		SingleTraceLock()    { std::cout << "Lock created!" << std::endl;   }
		~SingleTraceLock()   { std::cout << "Lock destroyed!" << std::endl; }
		inline void Lock()   { std::cout << "+++LOCK+++" << std::endl;      }
		inline void Unlock() { std::cout << "--UNLOCK--" << std::endl;      }
	private:
		SingleTraceLock(const SingleTraceLock&);
		SingleTraceLock& operator=(const SingleTraceLock&);
	};

	/////////////////////////////////////////////////////////////////////
	// Log definitions
	/////////////////////////////////////////////////////////////////////
	// собираем из полученных кубиков разные типы логгеров
	typedef Log<COUTWriter, SingleThreadLock> SingleCOUTLog;
	typedef Log<COUTWriter, CriticalSectionLock> SafeCOUTLog;
	typedef Log<COUTWriter, SingleTraceLock> SingleTraceLog;
	typedef Log<FileWriter, SingleTraceLock> SingleFileLog;
	typedef Log<FileWriter, CriticalSectionLock> SafeFileLog;

	typedef StreamLog<SingleCOUTLog> SingleStreamCOUTLog;
	typedef StreamLog<SafeCOUTLog> SafeStreamCOUTLog;
	typedef StreamLog<SingleTraceLog> SingleStreamTraceLog;
	typedef StreamLog<SingleFileLog> SingleStreamFileLog;
	typedef StreamLog<SafeFileLog> SafeStreamFileLog;

	/////////////////////////////////////////////////////////////////////
	// Log helpers
	/////////////////////////////////////////////////////////////////////

	namespace Private
	{
		// Синглтон от WolfHound: http://rsdn.ru/forum/Message.aspx?mid=531518&only=1
		inline LONG atom_set(volatile LONG* ptr, LONG value)
		{
			return ::InterlockedExchange(ptr, value);
		}

		inline LONG atom_get(volatile LONG* ptr)
		{
			return ::InterlockedExchangeAdd(ptr, 0);
		}

		struct auto_lock
		{
			auto_lock(LONG* ptr)
				: ptr_(ptr)
			{
				while(atom_set(ptr_, 1))
					::Sleep(1);
			}

			~auto_lock()
			{
				atom_set(ptr_, 0);
			}

			LONG* ptr_;
		};

		template <typename T>
		class SingletonHolder
		{
		public:
			static T& Instance()
			{
				static T* obj_ptr=0; //Статическая инициализация(потокобезопасно)
				static LONG flag=0;  //Аналогично
				T* ptr=(T*)atom_get((LONG*)&obj_ptr);
				if(!ptr)
				{
					auto_lock lock(&flag);
					ptr=(T*)atom_get((LONG*)&obj_ptr);
					if(!ptr)
					{
						static T obj; //Создастся при первом поподании сюда
						ptr=&obj;
						atom_set((LONG*)&obj_ptr, (LONG)ptr);
					}
				}
				return *ptr;
			}
		private:
			SingletonHolder();
			SingletonHolder(const SingletonHolder<T>&);
			SingletonHolder<T>& operator=(const SingletonHolder<T>&);
		};
	}

	template <typename SLog>
	inline SLog logDebug(int level = Debug)
	{
		return SLog(Private::SingletonHolder<typename SLog::LogType>::Instance(), level >= Debug ? level : Debug);
	}

	template <typename SLog>
	inline SLog logWarning()
	{
		return SLog(Private::SingletonHolder<typename SLog::LogType>::Instance(), Warning);
	}

	template <typename SLog>
	inline SLog logError()
	{
		return SLog(Private::SingletonHolder<typename SLog::LogType>::Instance(), Error);
	}

	template <typename SLog>
	inline void logLevel(int logLevel)
	{
		Private::SingletonHolder<typename SLog::LogType>::Instance().setLogLevel(logLevel);
	}

	template <typename SLog>
	inline void logPrefix(const std::string& logPrefix)
	{
		Private::SingletonHolder<typename SLog::LogType>::Instance().setLogPrefix(logPrefix);
	}

	template <typename SLog>
	inline bool logEnabled(int logLevel)
	{
		return Private::SingletonHolder<typename SLog::LogType>::Instance().canLog(logLevel);
	}

} // namespace log

// лог - потокобезопасная запись в файл
#define LOG_IMPLEMENTATION ::logger::SafeStreamFileLog

//#define LOG_IMPLEMENTATION ::logger::SingleStreamCOUTLog
//#define LOG_IMPLEMENTATION ::logger::SafeStreamCOUTLog
// для визуализации блокировок в окне консоли:
//#define LOG_IMPLEMENTATION ::logger::SingleStreamTraceLog

#if defined(LOG_DEBUG_DISABLED) || defined(LOG_DISABLED)
#  define LOG_DEBUG if(true); else ::logger::logDebug<LOG_IMPLEMENTATION>()
#  define LOG_DEBUG_LEV(L) if(true); else ::logger::logDebug<LOG_IMPLEMENTATION>(L)
#else
#  define LOG_DEBUG if(!::logger::logEnabled<LOG_IMPLEMENTATION>(::logger::Debug)); else ::logger::logDebug<LOG_IMPLEMENTATION>()
#  define LOG_DEBUG_LEV(L) if(!::logger::logEnabled<LOG_IMPLEMENTATION>(L)); else ::logger::logDebug<LOG_IMPLEMENTATION>(L)
#endif

#if defined(LOG_WARNING_DISABLED) || defined(LOG_DISABLED)
#  define LOG_WARNING if(true); else ::logger::logWarning<LOG_IMPLEMENTATION>()
#else
#  define LOG_WARNING if(!::logger::logEnabled<LOG_IMPLEMENTATION>(::logger::Warning)); else ::logger::logWarning<LOG_IMPLEMENTATION>()
#endif

#if defined(LOG_ERROR_DISABLED) || defined(LOG_DISABLED)
#  define LOG_ERROR if(true); else ::logger::logError<LOG_IMPLEMENTATION>()
#else
#  define LOG_ERROR if(!::logger::logEnabled<LOG_IMPLEMENTATION>(::logger::Error)); else ::logger::logError<LOG_IMPLEMENTATION>()
#endif

#if defined(LOG_DISABLED)
#  define LOG_LEVEL(L) if(true); else ::logger::logLevel<LOG_IMPLEMENTATION>(L)
#  define LOG_PREFIX(P) if(true); else ::logger::logPrefix<LOG_IMPLEMENTATION>(P)
#else
#  define LOG_LEVEL(L) ::logger::logLevel<LOG_IMPLEMENTATION>(L)
#  define LOG_PREFIX(P) ::logger::logPrefix<LOG_IMPLEMENTATION>(P)
#endif

# pragma warning(pop)

#endif // __LOGGER_H