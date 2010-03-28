object FormDisciplines: TFormDisciplines
  Left = 295
  Top = 137
  Width = 627
  Height = 533
  Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1076#1080#1089#1094#1080#1087#1083#1080#1085
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    619
    502)
  PixelsPerInch = 96
  TextHeight = 13
  object LabSpec: TLabel
    Left = 0
    Top = 457
    Width = 78
    Height = 13
    Anchors = [akLeft, akRight, akBottom]
    Caption = #1057#1087#1077#1094#1080#1072#1083#1100#1085#1086#1089#1090#1100
  end
  object LabDiscipline: TLabel
    Left = 0
    Top = 313
    Width = 63
    Height = 13
    Anchors = [akLeft, akRight, akBottom]
    Caption = #1044#1080#1089#1094#1080#1087#1083#1080#1085#1072
  end
  object BtnAdd: TSpeedButton
    Left = 512
    Top = 377
    Width = 105
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF003C3333339333
      337437FFF3337F3333F73CCC33339333344437773F337F33377733CCC3339337
      4447337F73FF7F3F337F33CCCCC3934444433373F7737F773373333CCCCC9444
      44733337F337773337F3333CCCCC9444443333373F337F3337333333CCCC9444
      473333337F337F337F333333CCCC94444333333373F37F33733333333CCC9444
      7333333337F37F37F33333333CCC944433333333373F7F373333333333CC9447
      33333333337F7F7F3333333333CC94433333333333737F7333333333333C9473
      33333333333737F333333333333C943333333333333737333333333333339733
      3333333333337F33333333333333933333333333333373333333}
    NumGlyphs = 2
    OnClick = BtnAddClick
  end
  object BtnDel: TSpeedButton
    Left = 512
    Top = 401
    Width = 105
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = #1059#1076#1072#1083#1080#1090#1100
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333393333
      333333333337F3333333333333397333333333333337FF333333333333C94333
      3333333333737F333333333333C9473333333333337373F3333333333CC94433
      3333333337F7F7F3333333333CC94473333333333737F73F33333333CCC94443
      333333337F37F37F33333333CCC94447333333337337F373F333333CCCC94444
      33333337F337F337F333333CCCC94444733333373337F3373F3333CCCCC94444
      4333337F3337FF337F3333CCCCC94444473333733F7773FF73F33CCCCC393444
      443337F37737F773F7F33CCC33393374447337F73337F33737FFCCC333393333
      444377733337F333777FC3333339333337437333333733333373}
    NumGlyphs = 2
    OnClick = BtnDelClick
  end
  object BtnEdit: TSpeedButton
    Left = 512
    Top = 425
    Width = 105
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = #1048#1079#1084#1077#1085#1080#1090#1100
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000000
      000033333377777777773333330FFFFFFFF03FF3FF7FF33F3FF700300000FF0F
      00F077F777773F737737E00BFBFB0FFFFFF07773333F7F3333F7E0BFBF000FFF
      F0F077F3337773F3F737E0FBFBFBF0F00FF077F3333FF7F77F37E0BFBF00000B
      0FF077F3337777737337E0FBFBFBFBF0FFF077F33FFFFFF73337E0BF0000000F
      FFF077FF777777733FF7000BFB00B0FF00F07773FF77373377373330000B0FFF
      FFF03337777373333FF7333330B0FFFF00003333373733FF777733330B0FF00F
      0FF03333737F37737F373330B00FFFFF0F033337F77F33337F733309030FFFFF
      00333377737FFFFF773333303300000003333337337777777333}
    NumGlyphs = 2
    OnClick = BtnEditClick
  end
  object BtnClose: TSpeedButton
    Left = 512
    Top = 473
    Width = 105
    Height = 22
    Anchors = [akRight, akBottom]
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
  object Label1: TLabel
    Left = 384
    Top = 313
    Width = 65
    Height = 13
    Anchors = [akRight, akBottom]
    Caption = #1040#1073#1088#1080#1074#1080#1072#1090#1091#1088#1072
  end
  object Label2: TLabel
    Left = 0
    Top = 357
    Width = 126
    Height = 13
    Anchors = [akLeft, akRight, akBottom]
    Caption = #1053#1072#1079#1085#1072#1095#1077#1085#1080#1077' '#1076#1080#1089#1094#1080#1087#1083#1080#1085#1099
  end
  object Label3: TLabel
    Left = 384
    Top = 357
    Width = 104
    Height = 13
    Anchors = [akRight, akBottom]
    Caption = #1055#1088#1086#1076#1086#1083#1078#1080#1090#1077#1083#1100#1085#1086#1089#1090#1100
  end
  object Label4: TLabel
    Left = 466
    Top = 376
    Width = 29
    Height = 13
    Anchors = [akRight, akBottom]
    Caption = #1095#1072#1089#1086#1074
  end
  object Label5: TLabel
    Left = 320
    Top = 357
    Width = 34
    Height = 13
    Anchors = [akRight, akBottom]
    Caption = #1055#1086#1088'.'#8470
  end
  object StrGridDiscip: TStringGrid
    Left = 0
    Top = 0
    Width = 619
    Height = 305
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clSilver
    ColCount = 6
    DefaultRowHeight = 16
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goColMoving, goRowSelect]
    TabOrder = 0
    OnClick = StrGridDiscipClick
  end
  object ComboBoxSpec: TComboBox
    Left = 0
    Top = 473
    Width = 505
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akRight, akBottom]
    Color = clSilver
    Enabled = False
    ItemHeight = 13
    TabOrder = 3
    OnChange = ComboBoxSpecChange
  end
  object ComboBoxDiscipline: TComboBox
    Left = 0
    Top = 329
    Width = 377
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    Color = clSilver
    ItemHeight = 13
    TabOrder = 1
  end
  object ComboBoxAbr: TComboBox
    Left = 384
    Top = 329
    Width = 121
    Height = 21
    Anchors = [akRight, akBottom]
    Color = clSilver
    ItemHeight = 13
    TabOrder = 2
  end
  object ComboBoxClassDiscip: TComboBox
    Left = 0
    Top = 373
    Width = 313
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akRight, akBottom]
    Color = clSilver
    ItemHeight = 13
    TabOrder = 4
    OnChange = ComboBoxClassDiscipChange
  end
  object EditNumHours: TEdit
    Left = 384
    Top = 373
    Width = 57
    Height = 21
    Anchors = [akRight, akBottom]
    Color = clSilver
    TabOrder = 5
    Text = '80'
    OnExit = EditNumHoursExit
  end
  object UpDownNumHours: TUpDown
    Left = 441
    Top = 373
    Width = 15
    Height = 21
    Anchors = [akRight, akBottom]
    Associate = EditNumHours
    Min = 0
    Max = 10000
    Position = 80
    TabOrder = 6
    Wrap = False
  end
  object EditScanNumber: TEdit
    Left = 320
    Top = 373
    Width = 41
    Height = 21
    Anchors = [akRight, akBottom]
    Color = clSilver
    TabOrder = 7
    Text = '0'
    OnExit = EditScanNumberExit
  end
  object UpDownScanNumber: TUpDown
    Left = 361
    Top = 373
    Width = 15
    Height = 21
    Anchors = [akRight, akBottom]
    Associate = EditScanNumber
    Min = 0
    Max = 10000
    Position = 0
    TabOrder = 8
    Wrap = False
  end
  object GroupBox5: TGroupBox
    Left = 0
    Top = 398
    Width = 505
    Height = 59
    Anchors = [akRight, akBottom]
    Caption = #1054#1073#1097#1080#1077' '#1095#1072#1089#1099' '#1085#1072' '#1082#1072#1078#1076#1099#1081' '#1089#1077#1084#1077#1089#1090#1088
    TabOrder = 9
    object Label6: TLabel
      Left = 8
      Top = 16
      Width = 6
      Height = 13
      Caption = '1'
    end
    object Label7: TLabel
      Left = 55
      Top = 16
      Width = 6
      Height = 13
      Caption = '2'
    end
    object Label8: TLabel
      Left = 99
      Top = 16
      Width = 6
      Height = 13
      Caption = '3'
    end
    object Label9: TLabel
      Left = 145
      Top = 16
      Width = 6
      Height = 13
      Caption = '4'
    end
    object Label10: TLabel
      Left = 190
      Top = 16
      Width = 6
      Height = 13
      Caption = '5'
    end
    object Label11: TLabel
      Left = 236
      Top = 16
      Width = 6
      Height = 13
      Caption = '6'
    end
    object Label12: TLabel
      Left = 281
      Top = 16
      Width = 6
      Height = 13
      Caption = '7'
    end
    object Label13: TLabel
      Left = 327
      Top = 16
      Width = 6
      Height = 13
      Caption = '8'
    end
    object Label14: TLabel
      Left = 372
      Top = 16
      Width = 6
      Height = 13
      Caption = '9'
    end
    object Label15: TLabel
      Left = 415
      Top = 16
      Width = 12
      Height = 13
      Caption = '10'
    end
    object Label16: TLabel
      Left = 462
      Top = 16
      Width = 12
      Height = 13
      Caption = '11'
    end
    object MaskEditAudHours1: TMaskEdit
      Left = 9
      Top = 30
      Width = 24
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 0
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours2: TMaskEdit
      Left = 54
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 1
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours3: TMaskEdit
      Left = 99
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 2
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours4: TMaskEdit
      Left = 145
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 3
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours5: TMaskEdit
      Left = 190
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 4
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours6: TMaskEdit
      Left = 236
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 5
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours7: TMaskEdit
      Left = 281
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 6
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours8: TMaskEdit
      Left = 326
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 7
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours9: TMaskEdit
      Left = 372
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 8
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours10: TMaskEdit
      Left = 417
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 9
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
    object MaskEditAudHours11: TMaskEdit
      Left = 463
      Top = 30
      Width = 25
      Height = 21
      EditMask = '!999;0;_'
      MaxLength = 3
      TabOrder = 10
      Text = '0  '
      OnExit = MaskEditAudHoursExit
    end
  end
end
