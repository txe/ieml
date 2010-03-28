object FormDataForDiplom: TFormDataForDiplom
  Left = 518
  Top = 84
  Width = 291
  Height = 206
  Caption = 'Задайте дополнительные данные'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 88
    Height = 20
    Caption = 'Диплом №'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 42
    Width = 183
    Height = 16
    Caption = 'регистрационный номер'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 75
    Width = 110
    Height = 13
    Caption = 'дата выдачи диплома'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 107
    Width = 170
    Height = 13
    Caption = 'дата присуждения квалификации'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object BtnOK: TSpeedButton
    Left = 96
    Top = 144
    Width = 81
    Height = 22
    Caption = 'Готово'
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
      555555555555555555555555555555555555555555FF55555555555559055555
      55555555577FF5555555555599905555555555557777F5555555555599905555
      555555557777FF5555555559999905555555555777777F555555559999990555
      5555557777777FF5555557990599905555555777757777F55555790555599055
      55557775555777FF5555555555599905555555555557777F5555555555559905
      555555555555777FF5555555555559905555555555555777FF55555555555579
      05555555555555777FF5555555555557905555555555555777FF555555555555
      5990555555555555577755555555555555555555555555555555}
    NumGlyphs = 2
    OnClick = BtnOKClick
  end
  object Bevel1: TBevel
    Left = 8
    Top = 136
    Width = 265
    Height = 2
  end
  object EditNumberDiplom: TEdit
    Left = 104
    Top = 8
    Width = 169
    Height = 21
    Color = clSilver
    TabOrder = 0
    Text = 'ВСВ 0774791'
  end
  object DateTimePickerDateVidachi: TDateTimePicker
    Left = 184
    Top = 72
    Width = 89
    Height = 21
    CalAlignment = dtaLeft
    Date = 38249.6226212384
    Time = 38249.6226212384
    Color = clSilver
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 1
  end
  object DateTimePickerDatePrisQualif: TDateTimePicker
    Left = 184
    Top = 104
    Width = 89
    Height = 21
    CalAlignment = dtaLeft
    Date = 38249.6226212384
    Time = 38249.6226212384
    Color = clSilver
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 2
  end
  object MaskEditRegNum: TEdit
    Left = 200
    Top = 40
    Width = 73
    Height = 21
    Color = clSilver
    TabOrder = 3
    Text = '123'
    OnEnter = MaskEditRegNumEnter
  end
end
