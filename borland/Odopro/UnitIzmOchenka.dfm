object FormIzmOchenka: TFormIzmOchenka
  Left = 169
  Top = 83
  Width = 738
  Height = 464
  Caption = #1048#1079#1084#1077#1085#1077#1085#1080#1077' '#1086#1094#1077#1085#1082#1080
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 304
    Top = 296
    Width = 105
    Height = 13
    Caption = #1042#1099#1073#1088#1072#1085#1085#1099#1081' '#1087#1088#1077#1076#1084#1077#1090
  end
  object StrGridDB: TStringGrid
    Left = 0
    Top = 0
    Width = 730
    Height = 281
    Hint = #1047#1072#1087#1080#1089#1080' '#1090#1072#1073#1083#1080#1094#1099
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clSilver
    ColCount = 6
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
      64
      64
      64)
    RowHeights = (
      16
      16)
  end
  object Button1: TButton
    Left = 637
    Top = 360
    Width = 75
    Height = 25
    Caption = #1047#1072#1084#1077#1085#1080#1090#1100
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 637
    Top = 392
    Width = 75
    Height = 25
    Caption = #1042#1099#1093#1086#1076
    TabOrder = 2
    OnClick = Button2Click
  end
  object MaskEditPredmet: TMaskEdit
    Left = 304
    Top = 312
    Width = 409
    Height = 21
    Hint = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1086#1085#1085#1099#1081' '#1085#1086#1084#1077#1088
    Color = clSilver
    MaxLength = 5
    ReadOnly = True
    TabOrder = 3
  end
  object RadioGroupType: TRadioGroup
    Left = 304
    Top = 344
    Width = 185
    Height = 73
    Caption = #1058#1080#1087' '#1086#1094#1077#1085#1082#1080
    Items.Strings = (
      #1069#1082#1079#1072#1084#1077#1085
      #1047#1072#1095#1077#1090)
    TabOrder = 4
  end
end
