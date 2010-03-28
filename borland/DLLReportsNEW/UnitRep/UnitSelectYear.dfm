object FormSelectYear: TFormSelectYear
  Left = 391
  Top = 360
  Width = 330
  Height = 148
  Caption = #1042#1099#1073#1086#1088' '#1091#1095#1077#1073#1085#1086#1075#1086' '#1075#1086#1076#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 160
    Top = 34
    Width = 18
    Height = 13
    Caption = '------'
  end
  object ComboBoxFirstYear: TComboBox
    Left = 24
    Top = 32
    Width = 113
    Height = 21
    ItemHeight = 13
    ItemIndex = 1
    TabOrder = 0
    Text = '2006'
    OnChange = ComboBoxFirstYearChange
    Items.Strings = (
      '2005'
      '2006'
      '2007'
      '2008'
      '2009'
      '2010'
      '2011'
      '2012')
  end
  object EditSecondYear: TEdit
    Left = 200
    Top = 32
    Width = 89
    Height = 21
    ReadOnly = True
    TabOrder = 1
    Text = '2007'
  end
  object Button1: TButton
    Left = 120
    Top = 72
    Width = 75
    Height = 25
    Caption = #1042#1099#1073#1088#1072#1090#1100
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 216
    Top = 72
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 3
  end
end
