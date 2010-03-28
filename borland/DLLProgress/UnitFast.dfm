object FormFast: TFormFast
  Left = 418
  Top = 117
  Width = 422
  Height = 598
  Caption = 'FormFast'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  OnResize = FormResize
  DesignSize = (
    414
    564)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelStuff: TLabel
    Left = 8
    Top = 459
    Width = 48
    Height = 13
    Anchors = [akBottom]
    Caption = 'LabelStuff'
  end
  object StrGridListOcenk: TStringGrid
    Left = 0
    Top = 0
    Width = 414
    Height = 449
    Hint = #1057#1087#1080#1089#1086#1082' '#1086#1094#1077#1085#1086#1082' '#1089#1090#1091#1076#1077#1085#1090#1072' '#1087#1086' '#1074#1089#1077#1084' '#1076#1080#1089#1094#1080#1087#1083#1080#1085#1072#1084' '#1079#1072' '#1074#1099#1073#1088#1072#1085#1085#1099#1081' '#1089#1077#1084#1077#1089#1090#1088
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clSilver
    ColCount = 3
    DefaultRowHeight = 16
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goRowMoving, goColMoving, goEditing, goAlwaysShowEditor]
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnDrawCell = StrGridListOcenkDrawCell
    OnKeyPress = StrGridListOcenkKeyPress
    OnSelectCell = StrGridListOcenkSelectCell
  end
  object ButtonSave: TButton
    Left = 328
    Top = 499
    Width = 77
    Height = 23
    Anchors = [akBottom]
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 1
    OnClick = ButtonSaveClick
  end
  object ButtonClose: TButton
    Left = 328
    Top = 531
    Width = 73
    Height = 23
    Anchors = [akBottom]
    Caption = #1047#1072#1082#1088#1099#1090#1100
    ModalResult = 1
    TabOrder = 2
  end
  object ButtonCopy: TButton
    Left = 8
    Top = 528
    Width = 75
    Height = 25
    Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 3
    OnClick = ButtonCopyClick
  end
  object ButtonInsert: TButton
    Left = 104
    Top = 528
    Width = 75
    Height = 25
    Caption = #1042#1089#1090#1072#1074#1080#1090#1100
    TabOrder = 4
    OnClick = ButtonInsertClick
  end
end
