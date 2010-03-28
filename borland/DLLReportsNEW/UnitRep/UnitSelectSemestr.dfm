object FormSelectSemestr: TFormSelectSemestr
  Left = 255
  Top = 355
  ActiveControl = Button1
  BorderStyle = bsToolWindow
  Caption = #1042#1099#1073#1086#1088' '#1089#1077#1084#1077#1089#1090#1088#1072
  ClientHeight = 29
  ClientWidth = 304
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 104
    Height = 13
    Caption = #1057#1077#1084#1077#1089#1090#1088' '#1087#1086' '#1075#1088#1072#1092#1080#1082#1091
  end
  object EditSemestrGraphic: TEdit
    Left = 120
    Top = 4
    Width = 33
    Height = 21
    ReadOnly = True
    TabOrder = 0
    Text = '1'
  end
  object UpDownGraphic: TUpDown
    Left = 153
    Top = 4
    Width = 15
    Height = 21
    Associate = EditSemestrGraphic
    Min = 1
    Max = 12
    Position = 1
    TabOrder = 1
    Wrap = False
  end
  object Button1: TButton
    Left = 184
    Top = 4
    Width = 108
    Height = 21
    Caption = #1042#1099#1073#1088#1072#1090#1100
    ModalResult = 1
    TabOrder = 2
  end
end
