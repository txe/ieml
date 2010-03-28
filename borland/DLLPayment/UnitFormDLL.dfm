object FormDLL: TFormDLL
  Left = 230
  Top = 153
  Width = 521
  Height = 406
  Caption = 'FormDLL'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxOpts: TGroupBox
    Left = 0
    Top = 0
    Width = 513
    Height = 89
    Hint = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1086#1087#1083#1072#1090#1099
    Align = alTop
    Caption = ' '#1042#1099#1073#1088#1072#1085#1085#1072#1103' '#1082#1072#1090#1077#1075#1086#1088#1080#1103' '#1086#1087#1083#1072#1090#1099' '
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    DesignSize = (
      513
      89)
    object BtnOptsOplata: TSpeedButton
      Left = 334
      Top = 11
      Width = 169
      Height = 22
      Hint = 
        #1065#1077#1083#1082#1085#1080#1090#1077' '#1076#1083#1103' '#1074#1099#1073#1086#1088#1072' '#1085#1077#1086#1073#1093#1086#1076#1080#1084#1086#1081' '#1082#1072#1090#1077#1075#1086#1088#1080#1080' '#1086#1087#1083#1072#1090#1099' '#1080#1083#1080' '#1076#1083#1103' '#1088#1077#1076#1072#1082#1090#1080 +
        #1088#1086#1074#1072#1085#1080#1103' '#1082#1072#1090#1077#1075#1086#1088#1080#1081' '#1086#1087#1083#1072#1090#1099
      Anchors = [akTop, akRight]
      Caption = #1042#1099#1073#1086#1088' '#1082#1072#1090#1077#1075#1086#1088#1080#1080' '#1086#1087#1083#1072#1090#1099'...'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00300000000000
        003337777777777777F330FFFFFFFFFFF03337F3333FFF3337F330FFFF000FFF
        F03337F33377733337F330FFFFF0FFFFF03337F33337F33337F330FFFF00FFFF
        F03337F33377F33337F330FFFFF0FFFFF03337F33337333337F330FFFFFFFFFF
        F03337FFF3F3F3F3F7F33000F0F0F0F0F0333777F7F7F7F7F7F330F0F000F070
        F03337F7F777F777F7F330F0F0F0F070F03337F7F7373777F7F330F0FF0FF0F0
        F03337F733733737F7F330FFFFFFFF00003337F33333337777F330FFFFFFFF0F
        F03337FFFFFFFF7F373330999999990F033337777777777F733330FFFFFFFF00
        333337FFFFFFFF77333330000000000333333777777777733333}
      NumGlyphs = 2
      OnClick = BtnOptsOplataClick
    end
    object LabOpts: TLabel
      Left = 8
      Top = 16
      Width = 109
      Height = 13
      Caption = #1042#1099#1073#1088#1072#1085#1085#1072' '#1082#1072#1090#1077#1075#1086#1088#1080#1103':'
    end
    object LabGroup: TLabel
      Left = 24
      Top = 32
      Width = 38
      Height = 13
      Caption = #1043#1088#1091#1087#1087#1072':'
    end
    object LabDates: TLabel
      Left = 24
      Top = 48
      Width = 81
      Height = 13
      Caption = #1055#1077#1088#1080#1086#1076' '#1086#1087#1083#1072#1090#1099':'
    end
    object LabMoney: TLabel
      Left = 24
      Top = 64
      Width = 91
      Height = 13
      Caption = #1057#1091#1084#1084#1072' '#1079#1072' '#1087#1077#1088#1080#1086#1076':'
    end
    object LabGroupVal: TLabel
      Left = 128
      Top = 32
      Width = 74
      Height = 13
      Hint = #1043#1088#1091#1087#1087#1072' ('#1086#1073#1103#1079#1072#1090#1077#1083#1100#1085#1086' '#1087#1088#1086#1074#1077#1088#1100#1090#1077' '#1080' '#1091#1090#1086#1095#1085#1080#1090#1077' '#1075#1088#1091#1087#1087#1091')'
      Caption = 'LabGroupVal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabDatesVal: TLabel
      Left = 128
      Top = 48
      Width = 73
      Height = 13
      Hint = #1055#1077#1088#1080#1086#1076' '#1086#1087#1083#1072#1090#1099
      Caption = 'LabDatesVal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LabMoneyVal: TLabel
      Left = 128
      Top = 64
      Width = 77
      Height = 13
      Hint = #1054#1073#1097#1072#1103' '#1089#1091#1084#1084#1072' '#1079#1072' '#1087#1077#1088#1080#1086#1076' '#1086#1087#1083#1072#1090#1099
      Caption = 'LabMoneyVal'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object GroupBoxOplata: TGroupBox
    Left = 0
    Top = 241
    Width = 513
    Height = 65
    Hint = 
      #1047#1076#1077#1089#1100' '#1084#1086#1078#1085#1086' '#1079#1072#1085#1086#1089#1080#1090#1100' '#1080#1085#1092#1086#1088#1084#1072#1094#1080#1102' '#1086#1073' '#1086#1087#1083#1072#1090#1077' '#1089#1090#1091#1076#1077#1085#1090#1086#1074' ('#1076#1086#1089#1090#1091#1087#1085#1086'. '#1090 +
      #1086#1083#1100#1082#1086' '#1077#1089#1083#1080' '#1074#1099#1073#1088#1072#1085#1072' '#1086#1076#1085#1072' '#1080#1079' '#1082#1072#1090#1077#1075#1086#1088#1080#1081' '#1086#1087#1083#1072#1090#1099')'
    Align = alTop
    Caption = '  '#1054#1087#1083#1072#1090#1072' '#1079#1072' '#1086#1073#1091#1095#1077#1085#1080#1077'  '
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    DesignSize = (
      513
      65)
    object LabDateOplata: TLabel
      Left = 8
      Top = 16
      Width = 66
      Height = 13
      Caption = #1044#1072#1090#1072' '#1086#1087#1083#1072#1090#1099
    end
    object BtnOplataFact: TSpeedButton
      Left = 374
      Top = 16
      Width = 129
      Height = 38
      Hint = #1053#1072#1078#1084#1080#1090#1077' '#1076#1083#1103' '#1087#1086#1076#1090#1074#1077#1088#1078#1076#1077#1085#1080#1103' '#1092#1072#1082#1090#1072' '#1086#1087#1083#1072#1090#1099
      Anchors = [akTop, akRight]
      Caption = #1054#1087#1083#1072#1090#1080#1090#1100'...'
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00500000000055
        555557777777775F55550FFFFFFFFF0555557F5555555F7FFF5F0FEEEEEE0000
        05007F555555777775770FFFFFF0BFBFB00E7F5F5557FFF557770F0EEEE000FB
        FB0E7F75FF57775555770FF00F0FBFBFBF0E7F57757FFFF555770FE0B00000FB
        FB0E7F575777775555770FFF0FBFBFBFBF0E7F5575FFFFFFF5770FEEE0000000
        FB0E7F555777777755770FFFFF0B00BFB0007F55557577FFF7770FEEEEE0B000
        05557F555557577775550FFFFFFF0B0555557FF5F5F57575F55500F0F0F0F0B0
        555577F7F7F7F7F75F5550707070700B055557F7F7F7F7757FF5507070707050
        9055575757575757775505050505055505557575757575557555}
      NumGlyphs = 2
      OnClick = BtnOplataFactClick
    end
    object LabMoneyOplata: TLabel
      Left = 200
      Top = 16
      Width = 74
      Height = 13
      Caption = #1057#1091#1084#1084#1072' '#1086#1087#1083#1072#1090#1099
    end
    object DateTimePickerOplata: TDateTimePicker
      Left = 8
      Top = 32
      Width = 185
      Height = 21
      Hint = 
        #1044#1072#1090#1072' '#1086#1087#1083#1072#1090#1099' ('#1080#1089#1087#1086#1083#1100#1079#1091#1077#1090#1089#1103' '#1090#1086#1083#1100#1082#1086' '#1074' '#1082#1072#1095#1077#1089#1090#1074#1077' '#1076#1086#1087#1086#1083#1100#1085#1080#1090#1077#1083#1100#1085#1086#1081' '#1080#1085#1092#1086 +
        #1088#1084#1072#1094#1080#1080')'
      CalAlignment = dtaLeft
      Date = 37802.6526283449
      Time = 37802.6526283449
      Color = clSilver
      DateFormat = dfShort
      DateMode = dmComboBox
      Kind = dtkDate
      ParseInput = False
      TabOrder = 0
    end
    object ComboBoxMoney: TComboBox
      Left = 200
      Top = 32
      Width = 167
      Height = 21
      Hint = #1057#1091#1084#1084#1072' '#1090#1077#1082#1091#1097#1077#1075#1086' '#1074#1079#1085#1086#1089#1072' '#1074' '#1088#1091#1073'.'
      Anchors = [akLeft, akTop, akRight]
      Color = clSilver
      ItemHeight = 13
      TabOrder = 1
      Items.Strings = (
        '1'
        '2'
        '3'
        '4'
        '5'
        '6'
        '7'
        '8'
        '9')
    end
  end
  object GroupBoxPrevOplata: TGroupBox
    Left = 0
    Top = 89
    Width = 513
    Height = 152
    Align = alTop
    Caption = '  '#1055#1088#1086#1096#1083#1099#1077' '#1086#1087#1083#1072#1090#1099' '#1079#1072' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' '#1087#1077#1088#1080#1086#1076'  '
    TabOrder = 1
    DesignSize = (
      513
      152)
    object LabDolg: TLabel
      Left = 374
      Top = 80
      Width = 80
      Height = 13
      Anchors = [akTop, akRight]
      Caption = #1047#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1100
    end
    object BtnDel: TSpeedButton
      Left = 374
      Top = 122
      Width = 129
      Height = 22
      Hint = #1059#1076#1072#1083#1080#1090#1100' '#1079#1072#1087#1080#1089#1100' '#1074' '#1090#1072#1073#1083#1080#1094#1077' '#1087#1088#1077#1076#1099#1076#1091#1097#1080#1093' '#1074#1099#1087#1083#1072#1090' '#1079#1072' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' '#1087#1077#1088#1080#1086#1076
      Anchors = [akRight, akBottom]
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1092#1072#1082#1090' '#1086#1087#1083#1072#1090#1099
      ParentShowHint = False
      ShowHint = True
      OnClick = BtnDelClick
    end
    object LabSumMoney: TLabel
      Left = 374
      Top = 40
      Width = 74
      Height = 13
      Anchors = [akTop, akRight]
      Caption = #1057#1091#1084#1084#1072' '#1074#1099#1087#1083#1072#1090
    end
    object Label1: TLabel
      Left = 486
      Top = 60
      Width = 20
      Height = 13
      Anchors = [akTop, akRight]
      Caption = #1088#1091#1073'.'
    end
    object Label2: TLabel
      Left = 485
      Top = 101
      Width = 20
      Height = 13
      Anchors = [akTop, akRight]
      Caption = #1088#1091#1073'.'
    end
    object BtnAllFacts: TSpeedButton
      Left = 374
      Top = 11
      Width = 129
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1074#1089#1077' '#1074#1099#1087#1083#1072#1090#1099
      OnClick = BtnAllFactsClick
    end
    object ListBoxPrevOplata: TListBox
      Left = 8
      Top = 16
      Width = 359
      Height = 129
      Hint = 
        #1057#1087#1080#1089#1086#1082' '#1087#1088#1077#1076#1099#1076#1091#1097#1080#1093' '#1074#1099#1087#1083#1072#1090' '#1076#1083#1103' '#1074#1099#1073#1088#1072#1085#1085#1086#1081' '#1082#1072#1090#1077#1075#1086#1088#1080#1080' '#1086#1087#1083#1072#1090#1099' ('#1077#1089#1083#1080' '#1090#1072 +
        #1082#1086#1074#1099#1077' '#1077#1089#1090#1100' '#1074' '#1073#1072#1079#1077' '#1076#1072#1085#1085#1099#1093')'
      Anchors = [akLeft, akTop, akRight, akBottom]
      Color = clSilver
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 13
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = ListBoxPrevOplataClick
    end
    object EditDolg: TEdit
      Left = 374
      Top = 96
      Width = 105
      Height = 21
      Hint = 
        #1047#1072#1076#1086#1083#1078#1077#1085#1085#1086#1089#1090#1100' '#1087#1086' '#1086#1087#1083#1072#1090#1077' '#1079#1072' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' '#1087#1077#1088#1080#1086#1076' ('#1090'.'#1077'. '#1089#1091#1084#1084#1072', '#1082#1086#1090#1086#1088#1091#1102 +
        ' '#1085#1077#1086#1073#1093#1086#1076#1080#1084#1086' '#1087#1086#1075#1072#1089#1080#1090#1100' '#1082' '#1082#1086#1085#1094#1091' '#1074#1099#1073#1088#1072#1085#1085#1086#1075#1086' '#1087#1077#1088#1080#1086#1076#1072')'
      Anchors = [akTop, akRight]
      Color = clSilver
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 2
    end
    object EditSumMoney: TEdit
      Left = 374
      Top = 56
      Width = 105
      Height = 21
      Hint = #1057#1091#1084#1084#1072' '#1074#1099#1087#1083#1072#1090' '#1079#1072' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' '#1087#1077#1088#1080#1086#1076
      TabStop = False
      Anchors = [akTop, akRight]
      Color = clSilver
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 1
    end
  end
end
