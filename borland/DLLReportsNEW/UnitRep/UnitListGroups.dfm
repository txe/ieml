object FormListGroup: TFormListGroup
  Left = 278
  Top = 197
  BorderStyle = bsDialog
  Caption = #1057#1087#1080#1089#1086#1082' '#1075#1088#1091#1087#1087
  ClientHeight = 215
  ClientWidth = 129
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedBtnGo: TSpeedButton
    Left = 0
    Top = 192
    Width = 129
    Height = 22
    Caption = #1043#1086#1090#1086#1074#1086
    Flat = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = SpeedBtnGoClick
  end
  object CheckListBox: TCheckListBox
    Left = 0
    Top = 0
    Width = 129
    Height = 189
    Align = alTop
    ItemHeight = 13
    TabOrder = 0
  end
end
