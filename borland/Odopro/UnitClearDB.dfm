object FormClearDB: TFormClearDB
  Left = 303
  Top = 304
  BorderStyle = bsSingle
  Caption = 'Удаление лишних записей в базе дынных'
  ClientHeight = 259
  ClientWidth = 352
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 337
    Height = 109
    Caption = ' Записи, помеченные на удаление '
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 48
      Width = 41
      Height = 13
      Caption = 'Данные'
    end
    object Bevel1: TBevel
      Left = 56
      Top = 22
      Width = 2
      Height = 75
    end
    object BtnDel: TSpeedButton
      Left = 240
      Top = 77
      Width = 89
      Height = 22
      Caption = 'Удалить'
      Flat = True
      OnClick = BtnDelClick
    end
    object Bevel2: TBevel
      Left = 232
      Top = 22
      Width = 2
      Height = 75
    end
    object CheckListBoxTabs: TCheckListBox
      Left = 64
      Top = 20
      Width = 161
      Height = 81
      BorderStyle = bsNone
      Color = clBtnFace
      ItemHeight = 13
      Items.Strings = (
        'Личные данные студентов'
        'Словарь терминов'
        'Категории оплаты'
        'Факты оплаты'
        'Дисциплины'
        'Успеваемость студентов')
      TabOrder = 0
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 128
    Width = 337
    Height = 105
    Caption = '  Уже потерянные цепочки данных  '
    TabOrder = 1
  end
end
