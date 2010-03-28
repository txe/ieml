object SplashForm: TSplashForm
  Left = 347
  Top = 226
  BorderStyle = bsNone
  ClientHeight = 186
  ClientWidth = 334
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 334
    Height = 186
    Align = alClient
    BevelInner = bvLowered
    BevelWidth = 2
    TabOrder = 0
    DesignSize = (
      334
      186)
    object Image: TImage
      Left = 4
      Top = 4
      Width = 326
      Height = 178
      Align = alClient
      AutoSize = True
      Center = True
      IncrementalDisplay = True
      OnClick = TimerTimer
    end
    object Label1: TLabel
      Left = 16
      Top = 165
      Width = 224
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093' MySQL'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      Transparent = True
    end
  end
  object Timer: TTimer
    Interval = 5000
    OnTimer = TimerTimer
    Left = 8
    Top = 8
  end
end
