object FormAccount: TFormAccount
  Left = 332
  Top = 397
  Width = 270
  Height = 162
  Caption = '�������� ���������'
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
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 241
    Height = 81
    Caption = ' ������� ����� � ������ '
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 24
      Width = 31
      Height = 13
      Caption = '�����'
    end
    object Label2: TLabel
      Left = 16
      Top = 53
      Width = 38
      Height = 13
      Caption = '������'
    end
    object EditLogin: TEdit
      Left = 64
      Top = 19
      Width = 161
      Height = 21
      TabOrder = 0
    end
    object EditPassword: TEdit
      Left = 64
      Top = 48
      Width = 161
      Height = 21
      PasswordChar = '*'
      TabOrder = 1
    end
  end
  object BitBtn1: TBitBtn
    Left = 144
    Top = 96
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = BitBtn1Click
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object BitBtn2: TBitBtn
    Left = 32
    Top = 96
    Width = 75
    Height = 25
    Caption = '������'
    TabOrder = 2
    Kind = bkCancel
  end
end
