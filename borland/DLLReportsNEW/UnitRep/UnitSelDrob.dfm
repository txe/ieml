object FormSelDrob: TFormSelDrob
  Left = 264
  Top = 214
  Width = 237
  Height = 94
  Caption = 'Выберите соотношение'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object ComboBox1: TComboBox
    Left = 8
    Top = 8
    Width = 209
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
    Items.Strings = (
      '5 / 12 + 7 / 12'
      '1 / 2 +1 / 2')
  end
  object BitBtn1: TBitBtn
    Left = 72
    Top = 32
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 1
    OnClick = BitBtn1Click
  end
end
