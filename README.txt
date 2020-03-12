- если не получается восстановить из бекапа, то установить max_allowed_packet = 16M
- перед комипиляцией отчетов в Borland требуется установить zeosdb по пути "$(BCB)\My\ZEOSDB";
- т.к. zeosdb использует старые версии клиента MySql, то в конфигурации Mysql сервера требуется установить поддержку старых версий в sql-mode;
- отчеты берут информацию о месторасположении базы из реестра;



//***  для служебных целей  ***//

  ALTER TABLE `students` ADD `zakaz_fio` TEXT NULL;
  ALTER TABLE `students` ADD `zakaz_by` TEXT NULL;
  ALTER TABLE `students` ADD `zakaz_doc` TEXT NULL;
  ALTER TABLE `students` ADD `zakaz_details` TEXT NULL;



ALTER TABLE `payfactstest` ADD COLUMN `ordernum` int(11) NULL DEFAULT 0 AFTER `moneypay`;

CREATE TABLE `group_info` (                                                  
                `grpid` int(11) NOT NULL,                                        
                `facultyid` int(11) NOT NULL,                                       
                `deleted` tinyint(1) NOT NULL default '0',                                   
                PRIMARY KEY  (`grpid`),
					 INDEX `indx_facultyid` (`facultyid`, `deleted`)                                                                                                       
              ) ENGINE=InnoDB ;






// устарело
ALTER TABLE `students`	CHANGE COLUMN `edunumreg` `edunumreg` VARCHAR(7) NULL DEFAULT NULL AFTER `edunumprilog`;

ALTER TABLE `students` ADD COLUMN `passkod` VARCHAR(10) NULL DEFAULT NULL AFTER `passnum`;


ALTER TABLE `students` CHANGE COLUMN `edunumdiplom` `edunumdiplom` VARCHAR(15) NULL DEFAULT NULL AFTER `roleid`;
ALTER TABLE `students` ADD COLUMN `edunumprilog` VARCHAR(15) NULL DEFAULT NULL AFTER `edunumdiplom`;


ALTER TABLE students ADD COLUMN perevod_na_kurs text DEFAULT NULL

не забыть изменить odoportal.cfg на config.ini и добавить секцию [link]

зачетные единицы
ALTER TABLE disciplines ADD COLUMN zachet_edinica INT(11) NOT NULL DEFAULT '1'

гражданство
ALTER TABLE students ADD COLUMN citizenryid INT(11) NOT NULL DEFAULT '1'
добавить в словарь русский с номером 1

категория
ALTER TABLE students ADD COLUMN category_num INT(1) NOT NULL DEFAULT '0'


DESCRIBE  students;
ALTER TABLE students MODIFY egereg VARCHAR(15) NOT NULL DEFAULT "00-000000000-00";

 CREATE TABLE `paypersonaltest` (                                                  
                `id` int(11) NOT NULL auto_increment,                                        
                `idstud` int(11) NOT NULL default '0',                                       
                `idopts` int(11) NOT NULL default '0',                  
                `commoncountmoney` int(11) NOT NULL default '0',                                                                          
                `deleted` tinyint(1) NOT NULL default '0',                                   
                PRIMARY KEY  (`id`)                                                                                                       
              ) ENGINE=InnoDB ;

ALTER TABLE payoptstest ADD COLUMN half_year INT(1) NOT NULL DEFAULT '0' AFTER commoncountmoney

