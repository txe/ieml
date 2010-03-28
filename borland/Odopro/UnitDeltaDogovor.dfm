object FormDeltaDogovor: TFormDeltaDogovor
  Left = 413
  Top = 349
  Width = 335
  Height = 106
  Caption = #1057#1084#1077#1097#1077#1085#1080#1077' '#1076#1083#1103' '#1044#1086#1075#1086#1074#1086#1088#1072
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
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 125
    Height = 13
    Caption = #1057#1084#1077#1097#1077#1085#1080#1077' '#1087#1086' '#1074#1077#1088#1090#1080#1082#1072#1083#1080
  end
  object Label2: TLabel
    Left = 16
    Top = 48
    Width = 136
    Height = 13
    Caption = #1057#1084#1077#1097#1077#1085#1080#1077' '#1087#1086' '#1075#1086#1088#1080#1079#1086#1085#1090#1072#1083#1080
  end
  object MaskEditVert: TMaskEdit
    Left = 160
    Top = 8
    Width = 31
    Height = 21
    EditMask = '!###;1;_'
    MaxLength = 3
    TabOrder = 0
    Text = '   '
  end
  object MaskEditGor: TMaskEdit
    Left = 160
    Top = 40
    Width = 31
    Height = 21
    EditMask = '!###;1;_'
    MaxLength = 3
    TabOrder = 1
    Text = '   '
  end
  object Button1: TButton
    Left = 240
    Top = 8
    Width = 75
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    ModalResult = 1
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 240
    Top = 40
    Width = 75
    Height = 25
    Caption = #1042#1099#1093#1086#1076
    ModalResult = 2
    TabOrder = 3
  end
end
