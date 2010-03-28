object FormFilterComm: TFormFilterComm
  Left = 276
  Top = 403
  BorderStyle = bsNone
  Caption = 'Общий фильтр записей'
  ClientHeight = 229
  ClientWidth = 333
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 333
    Height = 229
    Align = alClient
    BevelInner = bvLowered
    TabOrder = 0
    object GroupBoxData: TGroupBox
      Left = 8
      Top = 81
      Width = 317
      Height = 65
      Caption = '   '
      Color = clBtnFace
      ParentColor = False
      TabOrder = 3
      object BtnFindData: TSpeedButton
        Left = 282
        Top = 19
        Width = 23
        Height = 22
        Flat = True
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000130B0000130B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          333333333333333FF3FF3333333333CC30003333333333773777333333333C33
          3000333FF33337F33777339933333C3333333377F33337F3333F339933333C33
          33003377333337F33377333333333C333300333F333337F33377339333333C33
          3333337FF3333733333F33993333C33333003377FF33733333773339933C3333
          330033377FF73F33337733339933C33333333FF377F373F3333F993399333C33
          330077F377F337F33377993399333C33330077FF773337F33377399993333C33
          33333777733337F333FF333333333C33300033333333373FF7773333333333CC
          3000333333333377377733333333333333333333333333333333}
        NumGlyphs = 2
        OnClick = BtnFindStrClick
      end
      object CheckBox2: TCheckBox
        Left = 11
        Top = 42
        Width = 294
        Height = 17
        Caption = 'По всем датам'
        Checked = True
        Enabled = False
        State = cbChecked
        TabOrder = 0
      end
      object DateTimePicker: TDateTimePicker
        Left = 11
        Top = 20
        Width = 270
        Height = 21
        CalAlignment = dtaLeft
        Date = 37712.6102456829
        Time = 37712.6102456829
        Color = clSilver
        DateFormat = dfShort
        DateMode = dmComboBox
        Kind = dtkDate
        ParseInput = False
        TabOrder = 1
      end
    end
    object GroupBoxStr: TGroupBox
      Left = 8
      Top = 8
      Width = 317
      Height = 65
      Caption = '  '
      Color = clBtnFace
      ParentColor = False
      TabOrder = 0
      object BtnFindStr: TSpeedButton
        Left = 282
        Top = 19
        Width = 23
        Height = 22
        Flat = True
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000130B0000130B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          333333333333333FF3FF3333333333CC30003333333333773777333333333C33
          3000333FF33337F33777339933333C3333333377F33337F3333F339933333C33
          33003377333337F33377333333333C333300333F333337F33377339333333C33
          3333337FF3333733333F33993333C33333003377FF33733333773339933C3333
          330033377FF73F33337733339933C33333333FF377F373F3333F993399333C33
          330077F377F337F33377993399333C33330077FF773337F33377399993333C33
          33333777733337F333FF333333333C33300033333333373FF7773333333333CC
          3000333333333377377733333333333333333333333333333333}
        NumGlyphs = 2
        OnClick = BtnFindStrClick
      end
      object ComboBoxSubStr: TComboBox
        Left = 11
        Top = 20
        Width = 270
        Height = 21
        Color = clSilver
        ItemHeight = 13
        TabOrder = 0
        Text = 'Лепсе'
      end
      object CheckBox1: TCheckBox
        Left = 11
        Top = 42
        Width = 294
        Height = 17
        Caption = 'В личных данных студентов'
        Checked = True
        Enabled = False
        State = cbChecked
        TabOrder = 1
      end
    end
    object RadioButtonStr: TRadioButton
      Left = 21
      Top = 6
      Width = 116
      Height = 17
      Caption = 'Поиск подстроки'
      Checked = True
      Color = clBtnFace
      ParentColor = False
      TabOrder = 1
      TabStop = True
      OnClick = RadioButtonStrClick
    end
    object RadioButtonData: TRadioButton
      Left = 21
      Top = 79
      Width = 89
      Height = 17
      Caption = 'Поиск даты'
      Color = clBtnFace
      ParentColor = False
      TabOrder = 2
      OnClick = RadioButtonStrClick
    end
    object GroupBoxDig: TGroupBox
      Left = 8
      Top = 154
      Width = 317
      Height = 65
      Caption = '   '
      Color = clBtnFace
      ParentColor = False
      TabOrder = 4
      object BtnFindDig: TSpeedButton
        Left = 282
        Top = 19
        Width = 23
        Height = 22
        Flat = True
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000130B0000130B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          333333333333333FF3FF3333333333CC30003333333333773777333333333C33
          3000333FF33337F33777339933333C3333333377F33337F3333F339933333C33
          33003377333337F33377333333333C333300333F333337F33377339333333C33
          3333337FF3333733333F33993333C33333003377FF33733333773339933C3333
          330033377FF73F33337733339933C33333333FF377F373F3333F993399333C33
          330077F377F337F33377993399333C33330077FF773337F33377399993333C33
          33333777733337F333FF333333333C33300033333333373FF7773333333333CC
          3000333333333377377733333333333333333333333333333333}
        NumGlyphs = 2
        OnClick = BtnFindStrClick
      end
      object CheckBox3: TCheckBox
        Left = 11
        Top = 42
        Width = 294
        Height = 17
        Caption = 'По всем номерам документов'
        Checked = True
        Enabled = False
        State = cbChecked
        TabOrder = 0
      end
      object ComboBoxDig: TComboBox
        Left = 11
        Top = 20
        Width = 270
        Height = 21
        Color = clSilver
        ItemHeight = 13
        TabOrder = 1
        Text = '123'
      end
    end
    object RadioButtonDig: TRadioButton
      Left = 21
      Top = 152
      Width = 92
      Height = 17
      Caption = 'Поиск числа'
      Color = clBtnFace
      ParentColor = False
      TabOrder = 5
      OnClick = RadioButtonStrClick
    end
  end
end
