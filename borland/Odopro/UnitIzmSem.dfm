object FormIzmSem: TFormIzmSem
  Left = 244
  Top = 165
  Width = 575
  Height = 452
  Caption = #1048#1079#1084#1077#1085#1077#1085#1080#1077' '#1087#1083#1072#1085'-'#1089#1077#1084#1077#1089#1090#1088#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 296
    Width = 105
    Height = 13
    Caption = #1042#1099#1073#1088#1072#1085#1085#1099#1081' '#1087#1088#1077#1076#1084#1077#1090
  end
  object LabelCurPlanSem: TLabel
    Left = 14
    Top = 344
    Width = 191
    Height = 13
    Caption = #1058#1077#1082#1091#1097#1080#1081' '#1087#1083#1072#1085'-'#1089#1077#1084#1077#1089#1090#1088' '#1076#1083#1103' '#1087#1088#1077#1076#1084#1077#1090#1072
  end
  object Label2: TLabel
    Left = 14
    Top = 375
    Width = 180
    Height = 13
    Caption = #1053#1086#1074#1099#1081' '#1087#1083#1072#1085'-'#1089#1077#1084#1077#1089#1090#1088' '#1076#1083#1103' '#1087#1088#1077#1076#1084#1077#1090#1072
  end
  object StrGridDB: TStringGrid
    Left = 0
    Top = 0
    Width = 567
    Height = 271
    Hint = #1047#1072#1087#1080#1089#1080' '#1090#1072#1073#1083#1080#1094#1099
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clSilver
    ColCount = 4
    DefaultRowHeight = 16
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnSelectCell = StrGridDBSelectCell
    ColWidths = (
      64
      64
      64
      64)
    RowHeights = (
      16
      16)
  end
  object MaskEditPredmet: TMaskEdit
    Left = 16
    Top = 312
    Width = 265
    Height = 21
    Hint = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1086#1085#1085#1099#1081' '#1085#1086#1084#1077#1088
    Color = clSilver
    MaxLength = 5
    ReadOnly = True
    TabOrder = 1
  end
  object MaskEditOldPlanSem: TMaskEdit
    Left = 240
    Top = 344
    Width = 38
    Height = 21
    Hint = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1086#1085#1085#1099#1081' '#1085#1086#1084#1077#1088
    Color = clSilver
    EditMask = '!99;1;_'
    MaxLength = 2
    ReadOnly = True
    TabOrder = 2
    Text = '  '
  end
  object MaskEditNewPlanSem: TMaskEdit
    Left = 240
    Top = 375
    Width = 38
    Height = 21
    Hint = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1086#1085#1085#1099#1081' '#1085#1086#1084#1077#1088
    Color = clSilver
    EditMask = '!99;1;_'
    MaxLength = 2
    TabOrder = 3
    Text = '  '
  end
  object Button1: TButton
    Left = 480
    Top = 344
    Width = 75
    Height = 25
    Caption = #1047#1072#1084#1077#1085#1080#1090#1100
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 480
    Top = 376
    Width = 75
    Height = 25
    Caption = #1042#1099#1093#1086#1076
    TabOrder = 5
    OnClick = Button2Click
  end
end
