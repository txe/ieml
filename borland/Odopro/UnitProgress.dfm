object FormProgress: TFormProgress
  Left = 331
  Top = 340
  BorderStyle = bsNone
  Caption = 'FormProgress'
  ClientHeight = 25
  ClientWidth = 322
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ProgressBar: TProgressBar
    Left = 0
    Top = 0
    Width = 322
    Height = 25
    Align = alClient
    Min = 0
    Max = 100
    Smooth = True
    Step = 1
    TabOrder = 0
  end
end
