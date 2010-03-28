object FormProgressDefSemestr: TFormProgressDefSemestr
  Left = 157
  Top = 285
  Width = 729
  Height = 265
  Caption = #1059#1089#1087#1077#1074#1072#1077#1084#1086#1089#1090#1100' '#1079#1072' '#1086#1087#1088#1077#1076#1077#1083#1077#1085#1085#1099#1081' '#1089#1077#1084#1077#1089#1090#1088
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    721
    232)
  PixelsPerInch = 96
  TextHeight = 13
  object BtnClose: TSpeedButton
    Left = 604
    Top = 208
    Width = 113
    Height = 22
    Hint = #1047#1072#1082#1088#1099#1090#1100' '#1086#1082#1085#1086' '#1091#1089#1087#1077#1074#1072#1077#1084#1086#1089#1090#1080
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
    ParentShowHint = False
    ShowHint = True
    OnClick = BtnCloseClick
  end
  object RadioButtonPlan: TRadioButton
    Left = 8
    Top = 208
    Width = 184
    Height = 17
    Hint = #1055#1077#1088#1077#1082#1083#1102#1095#1072#1090#1077#1083#1100' '#1085#1072' '#1074#1099#1073#1086#1088#1082#1091' '#1089#1077#1084#1077#1089#1090#1088#1072' '#1087#1086' '#1087#1083#1072#1085#1091
    Anchors = [akLeft, akBottom]
    Caption = #1057#1077#1084#1077#1089#1090#1088' '#1087#1086' '#1087#1083#1072#1085#1091
    Checked = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    TabStop = True
    OnClick = RadioButtonPlanClick
  end
  object RadioButtonGraph: TRadioButton
    Left = 232
    Top = 208
    Width = 184
    Height = 17
    Hint = #1055#1077#1088#1077#1082#1083#1102#1095#1072#1090#1077#1083#1100' '#1085#1072' '#1074#1099#1073#1086#1088#1082#1091' '#1089#1077#1084#1077#1089#1090#1088#1072' '#1087#1086' '#1075#1088#1072#1092#1080#1082#1091
    Anchors = [akLeft, akBottom]
    Caption = #1057#1077#1084#1077#1089#1090#1088' '#1087#1086' '#1075#1088#1072#1092#1080#1082#1091
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = RadioButtonPlanClick
  end
  object StrGridListOcenk: TStringGrid
    Left = 0
    Top = 0
    Width = 721
    Height = 201
    Hint = #1057#1087#1080#1089#1086#1082' '#1086#1094#1077#1085#1086#1082' '#1089#1090#1091#1076#1077#1085#1090#1072' '#1087#1086' '#1074#1089#1077#1084' '#1076#1080#1089#1094#1080#1087#1083#1080#1085#1072#1084' '#1079#1072' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' '#1089#1077#1084#1077#1089#1090#1088
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clSilver
    ColCount = 4
    DefaultRowHeight = 16
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowMoving, goColMoving, goRowSelect]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
  end
  object UpDownPlan: TUpDown
    Left = 176
    Top = 208
    Width = 15
    Height = 21
    Hint = #1053#1072#1074#1080#1075#1072#1090#1086#1088' '#1087#1086' '#1085#1086#1084#1077#1088#1091' '#1089#1077#1084#1077#1089#1090#1088#1072
    Anchors = [akLeft, akBottom]
    Associate = EditPlan
    Min = 1
    Max = 12
    ParentShowHint = False
    Position = 1
    ShowHint = True
    TabOrder = 3
    Wrap = False
    OnClick = UpDownPlanClick
  end
  object UpDownGraph: TUpDown
    Left = 400
    Top = 208
    Width = 15
    Height = 21
    Hint = #1053#1072#1074#1080#1075#1072#1090#1086#1088' '#1087#1086' '#1085#1086#1084#1077#1088#1091' '#1089#1077#1084#1077#1089#1090#1088#1072
    Anchors = [akLeft, akBottom]
    Associate = EditGraph
    Enabled = False
    Min = 1
    Max = 12
    ParentShowHint = False
    Position = 1
    ShowHint = True
    TabOrder = 4
    Wrap = False
    OnClick = UpDownPlanClick
  end
  object EditGraph: TEdit
    Left = 360
    Top = 208
    Width = 40
    Height = 21
    Hint = #1056#1077#1076#1072#1082#1090#1086#1088' '#1085#1086#1084#1077#1088#1072' '#1089#1077#1084#1077#1089#1090#1088#1072' '#1087#1086' '#1075#1088#1072#1092#1080#1082#1091
    Anchors = [akLeft, akBottom]
    Color = clSilver
    Enabled = False
    ParentShowHint = False
    ReadOnly = True
    ShowHint = True
    TabOrder = 5
    Text = '1'
  end
  object EditPlan: TEdit
    Left = 136
    Top = 208
    Width = 40
    Height = 21
    Hint = #1056#1077#1076#1072#1082#1090#1086#1088' '#1085#1086#1084#1077#1088#1072' '#1087#1083#1072#1085#1086#1074#1086#1075#1086' '#1089#1077#1084#1077#1089#1090#1088#1072' '
    Anchors = [akLeft, akBottom]
    Color = clSilver
    ParentShowHint = False
    ReadOnly = True
    ShowHint = True
    TabOrder = 6
    Text = '1'
  end
end
