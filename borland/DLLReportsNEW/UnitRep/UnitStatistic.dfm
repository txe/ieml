object FormStatistic: TFormStatistic
  Left = 361
  Top = 191
  Width = 607
  Height = 349
  Caption = #1057#1090#1072#1090#1080#1089#1090#1080#1082#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  DesignSize = (
    599
    315)
  PixelsPerInch = 96
  TextHeight = 13
  object BtnClose: TSpeedButton
    Left = 416
    Top = 288
    Width = 177
    Height = 22
    Anchors = [akTop, akRight]
    Caption = #1047#1072#1082#1088#1099#1090#1100
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
    OnClick = BtnCloseClick
  end
  object BtnStatistic: TSpeedButton
    Left = 232
    Top = 288
    Width = 177
    Height = 22
    Anchors = [akTop, akRight]
    Caption = #1042#1099#1074#1077#1089#1090#1080' '#1089#1090#1072#1090#1080#1089#1090#1080#1082#1091
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00337333733373
      3373337F3F7F3F7F3F7F33737373737373733F7F7F7F7F7F7F7F770000000000
      00007777777777777777330333333C333333337FFF3337F3333F370993333C33
      3399377773F337F33377330339333C3339333F7FF7FFF7FFF7FF770777977C77
      97777777777777777777330333933C339333337F3373F7F37333370333393C39
      3333377F333737F7333333033333999333333F7FFFFF777FFFFF770777777C77
      77777777777777777777330333333C330333337F333337FF7FF3370333333C00
      003C377F333337777737330333333C3303333F7FFFFFF7FF7FFF770777777777
      7777777777777777777733333333333333333333333333333333}
    NumGlyphs = 2
    OnClick = BtnStatisticClick
  end
  object Memo: TMemo
    Left = 8
    Top = 200
    Width = 185
    Height = 81
    TabOrder = 1
    Visible = False
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 585
    Height = 273
    Anchors = [akLeft, akTop, akRight]
    Caption = ' '#1050#1088#1080#1090#1077#1088#1080#1080' '#1092#1086#1088#1084#1080#1088#1086#1074#1072#1085#1080#1103' '#1089#1090#1072#1090#1080#1089#1090#1080#1095#1077#1089#1082#1080#1093' '#1076#1072#1085#1085#1099#1093'  '
    TabOrder = 0
    DesignSize = (
      585
      273)
    object ComboBoxgroup: TComboBox
      Left = 152
      Top = 160
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 0
    end
    object ComboBoxspec: TComboBox
      Left = 152
      Top = 184
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 1
    end
    object ComboBoxeduform: TComboBox
      Left = 152
      Top = 208
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 2
    end
    object ComboBoxdogfast: TComboBox
      Left = 152
      Top = 232
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 3
    end
    object ComboBoxdogyear: TComboBox
      Left = 152
      Top = 136
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 4
    end
    object ComboBoxSex: TComboBox
      Left = 152
      Top = 24
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 5
    end
    object MaskEditbyear: TMaskEdit
      Left = 152
      Top = 48
      Width = 417
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditMask = '!9999;1;_'
      MaxLength = 4
      TabOrder = 6
      Text = '    '
      OnExit = MaskEditbyearExit
    end
    object ComboBoxeducation: TComboBox
      Left = 152
      Top = 80
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 7
    end
    object ComboBoxlanguage: TComboBox
      Left = 152
      Top = 104
      Width = 417
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 8
    end
    object CheckBoxSex: TCheckBox
      Left = 16
      Top = 26
      Width = 130
      Height = 17
      Caption = #1055#1086#1083
      TabOrder = 9
      OnClick = CheckBoxSexClick
    end
    object CheckBoxbyear: TCheckBox
      Left = 16
      Top = 50
      Width = 130
      Height = 17
      Caption = #1043#1086#1076' '#1088#1086#1078#1076#1077#1085#1080#1103
      TabOrder = 10
      OnClick = CheckBoxSexClick
    end
    object CheckBoxeducation: TCheckBox
      Left = 16
      Top = 82
      Width = 130
      Height = 17
      Caption = #1054#1073#1088#1072#1079#1086#1074#1072#1085#1080#1077
      TabOrder = 11
      OnClick = CheckBoxSexClick
    end
    object CheckBoxlanguage: TCheckBox
      Left = 16
      Top = 106
      Width = 130
      Height = 17
      Caption = #1071#1079#1099#1082
      TabOrder = 12
      OnClick = CheckBoxSexClick
    end
    object CheckBoxdogyear: TCheckBox
      Left = 16
      Top = 138
      Width = 130
      Height = 17
      Caption = #1043#1086#1076' '#1087#1086#1089#1090#1091#1087#1083#1077#1085#1080#1103
      TabOrder = 13
      OnClick = CheckBoxSexClick
    end
    object CheckBoxgroup: TCheckBox
      Left = 16
      Top = 162
      Width = 130
      Height = 17
      Caption = #1043#1088#1091#1087#1087#1072
      TabOrder = 14
      OnClick = CheckBoxSexClick
    end
    object CheckBoxspec: TCheckBox
      Left = 16
      Top = 186
      Width = 130
      Height = 17
      Caption = #1057#1087#1077#1094#1080#1072#1083#1100#1085#1086#1089#1090#1100
      TabOrder = 15
      OnClick = CheckBoxSexClick
    end
    object CheckBoxeduform: TCheckBox
      Left = 16
      Top = 210
      Width = 130
      Height = 17
      Caption = #1060#1086#1088#1084#1072' '#1086#1073#1091#1095#1077#1085#1080#1103
      TabOrder = 16
      OnClick = CheckBoxSexClick
    end
    object CheckBoxdogfast: TCheckBox
      Left = 16
      Top = 234
      Width = 130
      Height = 17
      Caption = #1057#1082#1086#1088#1086#1089#1090#1100' '#1086#1073#1091#1095#1077#1085#1080#1103
      TabOrder = 17
      OnClick = CheckBoxSexClick
    end
  end
  object CheckBoxIgnore: TCheckBox
    Left = 24
    Top = 288
    Width = 201
    Height = 17
    Caption = #1048#1075#1085#1086#1088#1080#1088#1086#1074#1072#1090#1100' '#1086#1090#1095#1080#1089#1083#1077#1085#1085#1099#1093
    Checked = True
    State = cbChecked
    TabOrder = 2
  end
  object ZMySqlDatabase: TZConnection
    Protocol = 'mysql-4.0'
    Port = 3306
    AutoCommit = True
    ReadOnly = True
    TransactIsolationLevel = tiNone
    Connected = False
    SQLHourGlass = False
    Left = 400
    Top = 56
  end
  object ZMySqlQuery: TZQuery
    Connection = ZMySqlDatabase
    RequestLive = False
    CachedUpdates = False
    ParamCheck = True
    Params = <>
    ShowRecordTypes = [utUnmodified, utModified, utInserted]
    UpdateMode = umUpdateChanged
    WhereMode = wmWhereKeyOnly
    Options = [doCalcDefaults]
    Left = 432
    Top = 48
  end
end
