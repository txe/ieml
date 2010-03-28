object FormSelected: TFormSelected
  Left = 259
  Top = 154
  Width = 520
  Height = 492
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = '2'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  Scaled = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    512
    459)
  PixelsPerInch = 96
  TextHeight = 13
  object BtnOk: TSpeedButton
    Left = 288
    Top = 424
    Width = 105
    Height = 22
    Hint = #1047#1072#1085#1077#1089#1090#1080' '#1080#1079#1084#1077#1085#1077#1085#1080#1103' '#1074' '#1073#1072#1079#1091' '#1076#1072#1085#1085#1099#1093
    Anchors = [akRight, akBottom]
    Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000000
      000033333377777777773333330FFFFFFFF03FF3FF7FF33F3FF700300000FF0F
      00F077F777773F737737E00BFBFB0FFFFFF07773333F7F3333F7E0BFBF000FFF
      F0F077F3337773F3F737E0FBFBFBF0F00FF077F3333FF7F77F37E0BFBF00000B
      0FF077F3337777737337E0FBFBFBFBF0FFF077F33FFFFFF73337E0BF0000000F
      FFF077FF777777733FF7000BFB00B0FF00F07773FF77373377373330000B0FFF
      FFF03337777373333FF7333330B0FFFF00003333373733FF777733330B0FF00F
      0FF03333737F37737F373330B00FFFFF0F033337F77F33337F733309030FFFFF
      00333377737FFFFF773333303300000003333337337777777333}
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    OnClick = BtnOkClick
  end
  object BtnCancel: TSpeedButton
    Left = 400
    Top = 424
    Width = 105
    Height = 22
    Hint = #1054#1090#1084#1077#1085#1080#1090#1100' '#1080#1079#1084#1077#1085#1077#1085#1080#1077' '#1087#1086#1083#1077#1081' '#1080' '#1079#1072#1082#1088#1099#1090#1100
    Anchors = [akRight, akBottom]
    Caption = #1047#1072#1082#1088#1099#1090#1100
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333FFFFF3333333333999993333333333F77777FFF333333999999999
      3333333777333777FF3333993333339993333377FF3333377FF3399993333339
      993337777FF3333377F3393999333333993337F777FF333337FF993399933333
      399377F3777FF333377F993339993333399377F33777FF33377F993333999333
      399377F333777FF3377F993333399933399377F3333777FF377F993333339993
      399377FF3333777FF7733993333339993933373FF3333777F7F3399933333399
      99333773FF3333777733339993333339933333773FFFFFF77333333999999999
      3333333777333777333333333999993333333333377777333333}
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    OnClick = BtnCancelClick
  end
  object BtnWorkVoc: TSpeedButton
    Left = 8
    Top = 424
    Width = 105
    Height = 22
    Hint = #1056#1077#1076#1072#1082#1090#1086#1088' '#1089#1083#1086#1074#1072#1088#1103
    Anchors = [akLeft, akBottom]
    Caption = #1057#1083#1086#1074#1072#1088#1100
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      333333333333FF3333333333333C0C333333333333F777F3333333333CC0F0C3
      333333333777377F33333333C30F0F0C333333337F737377F333333C00FFF0F0
      C33333F7773337377F333CC0FFFFFF0F0C3337773F33337377F3C30F0FFFFFF0
      F0C37F7373F33337377F00FFF0FFFFFF0F0C7733373F333373770FFFFF0FFFFF
      F0F073F33373F333373730FFFFF0FFFFFF03373F33373F333F73330FFFFF0FFF
      00333373F33373FF77333330FFFFF000333333373F333777333333330FFF0333
      3333333373FF7333333333333000333333333333377733333333333333333333
      3333333333333333333333333333333333333333333333333333}
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    OnClick = BtnWorkVocClick
  end
  object BtnOkCancel: TSpeedButton
    Left = 176
    Top = 424
    Width = 105
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = #1043#1086#1090#1086#1074#1086
    Enabled = False
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
      555555555555555555555555555555555555555555FF55555555555559055555
      55555555577FF5555555555599905555555555557777F5555555555599905555
      555555557777FF5555555559999905555555555777777F555555559999990555
      5555557777777FF5555557990599905555555777757777F55555790555599055
      55557775555777FF5555555555599905555555555557777F5555555555559905
      555555555555777FF5555555555559905555555555555777FF55555555555579
      05555555555555777FF5555555555557905555555555555777FF555555555555
      5990555555555555577755555555555555555555555555555555}
    NumGlyphs = 2
    Visible = False
    OnClick = BtnOkCancelClick
  end
  object Memo1: TMemo
    Left = 176
    Top = 360
    Width = 225
    Height = 65
    ScrollBars = ssVertical
    TabOrder = 0
    Visible = False
  end
  object PageControlSelected: TPageControl
    Left = 0
    Top = 0
    Width = 512
    Height = 419
    ActivePage = TabSheetRelease
    Anchors = [akLeft, akTop, akRight, akBottom]
    MultiLine = True
    TabIndex = 3
    TabOrder = 1
    object TabSheetLich: TTabSheet
      Caption = #1051#1080#1095#1085#1099#1077' '#1076#1072#1085#1085#1099#1077
      object LabSecondName: TLabel
        Left = 8
        Top = 8
        Width = 49
        Height = 13
        Caption = #1060#1072#1084#1080#1083#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object LabFirstName: TLabel
        Left = 192
        Top = 8
        Width = 22
        Height = 13
        Caption = #1048#1084#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object LabThirdName: TLabel
        Left = 328
        Top = 8
        Width = 47
        Height = 13
        Caption = #1054#1090#1095#1077#1089#1090#1074#1086
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object LabSex: TLabel
        Left = 8
        Top = 48
        Width = 20
        Height = 13
        Caption = #1055#1086#1083
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labbdate: TLabel
        Left = 88
        Top = 48
        Width = 79
        Height = 13
        Caption = #1044#1072#1090#1072' '#1088#1086#1078#1076#1077#1085#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labpassseries: TLabel
        Left = 88
        Top = 104
        Width = 30
        Height = 13
        Caption = #1089#1077#1088#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        WordWrap = True
      end
      object Labpassnum: TLabel
        Left = 192
        Top = 104
        Width = 32
        Height = 13
        Caption = #1085#1086#1084#1077#1088
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labpassdate: TLabel
        Left = 328
        Top = 104
        Width = 66
        Height = 13
        Caption = #1044#1072#1090#1072' '#1074#1099#1076#1072#1095#1080
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labpassplace: TLabel
        Left = 88
        Top = 144
        Width = 56
        Height = 13
        Caption = #1050#1077#1084' '#1074#1099#1076#1072#1085
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labpass: TLabel
        Left = 8
        Top = 104
        Width = 46
        Height = 13
        Caption = #1055#1072#1089#1087#1086#1088#1090':'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labemail: TLabel
        Left = 250
        Top = 280
        Width = 31
        Height = 13
        Caption = 'E-mail:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labaddr: TLabel
        Left = 8
        Top = 200
        Width = 97
        Height = 13
        Caption = #1040#1076#1088#1077#1089' '#1087#1086' '#1087#1088#1086#1087#1080#1089#1082#1077
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labliveaddr: TLabel
        Left = 9
        Top = 240
        Width = 170
        Height = 13
        Caption = #1040#1076#1088#1077#1089' '#1092#1072#1082#1090#1080#1095#1077#1089#1082#1086#1075#1086' '#1087#1088#1086#1078#1080#1074#1072#1085#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labphones: TLabel
        Left = 9
        Top = 280
        Width = 53
        Height = 13
        Caption = #1058#1077#1083#1077#1092#1086#1085#1099
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Bevel1: TBevel
        Left = 8
        Top = 96
        Width = 489
        Height = 3
      end
      object Bevel2: TBevel
        Left = 8
        Top = 192
        Width = 489
        Height = 3
      end
      object EditSecondName: TEdit
        Left = 8
        Top = 24
        Width = 177
        Height = 21
        Hint = #1060#1072#1084#1080#1083#1080#1103' '
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        Text = #1048#1074#1072#1085#1086#1074
      end
      object EditFirstName: TEdit
        Left = 192
        Top = 24
        Width = 129
        Height = 21
        Hint = #1048#1084#1103
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = #1048#1074#1072#1085
      end
      object EditThirdName: TEdit
        Left = 328
        Top = 24
        Width = 169
        Height = 21
        Hint = #1054#1090#1095#1077#1089#1090#1074#1086
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        Text = #1048#1074#1072#1085#1086#1074#1080#1095
      end
      object ComboBoxSex: TComboBox
        Left = 8
        Top = 64
        Width = 73
        Height = 21
        Hint = #1055#1086#1083
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        Items.Strings = (
          #1052
          #1046)
      end
      object MaskEditbdate: TMaskEdit
        Left = 88
        Top = 64
        Width = 97
        Height = 21
        Hint = #1044#1072#1090#1072' '#1088#1086#1078#1076#1077#1085#1080#1103
        Color = clSilver
        EditMask = '!99/99/9999;1;_'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        MaxLength = 10
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = '  .  .    '
        OnExit = MaskEditbdateExit
      end
      object Editpassseries: TEdit
        Left = 88
        Top = 120
        Width = 97
        Height = 21
        Hint = #1057#1077#1088#1080#1103' '#1087#1072#1089#1087#1086#1088#1090#1072
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        Text = '22 97'
      end
      object Editpassnum: TEdit
        Left = 192
        Top = 120
        Width = 129
        Height = 21
        Hint = #1053#1086#1084#1077#1088' '#1087#1072#1089#1087#1086#1088#1090#1072
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '123456'
      end
      object Editpassplace: TEdit
        Left = 88
        Top = 160
        Width = 409
        Height = 21
        Hint = #1050#1077#1084' '#1074#1099#1076#1072#1085' '#1087#1072#1089#1087#1086#1088#1090
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 8
      end
      object MaskEditpassdate: TMaskEdit
        Left = 328
        Top = 120
        Width = 169
        Height = 21
        Hint = #1044#1072#1090#1072' '#1074#1099#1076#1072#1095#1080' '#1087#1072#1089#1087#1086#1088#1090#1072
        Color = clSilver
        EditMask = '!99/99/9999;1;_'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        MaxLength = 10
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
        Text = '  .  .    '
        OnExit = MaskEditbdateExit
      end
      object Editemail: TEdit
        Left = 248
        Top = 296
        Width = 249
        Height = 21
        Hint = #1040#1076#1088#1077#1089' '#1101#1083#1077#1082#1090#1088#1086#1085#1085#1086#1081' '#1087#1086#1095#1090#1099
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 12
        Text = 'qwe@mail.ru'
        OnExit = EditemailExit
      end
      object Editaddr: TEdit
        Left = 8
        Top = 216
        Width = 489
        Height = 21
        Hint = #1044#1086#1084#1072#1096#1085#1080#1081' '#1072#1076#1088#1077#1089' '#1087#1086' '#1087#1088#1086#1087#1080#1089#1082#1077
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 9
        Text = #1082#1086#1085#1090#1077#1081#1085#1077#1088' 2'
      end
      object Editliveaddr: TEdit
        Left = 8
        Top = 256
        Width = 489
        Height = 21
        Hint = #1040#1076#1088#1077#1089' '#1092#1072#1082#1090#1080#1095#1077#1089#1082#1086#1075#1086' '#1087#1088#1086#1078#1080#1074#1072#1085#1080#1103
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 10
        Text = #1082#1086#1085#1090#1077#1081#1085#1077#1088' 4'
      end
      object Editphones: TEdit
        Left = 8
        Top = 296
        Width = 233
        Height = 21
        Hint = #1058#1077#1083#1077#1092#1086#1085#1099' '#1076#1083#1103' '#1089#1074#1103#1079#1080
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 11
        Text = '02 03 04'
      end
    end
    object TabSheetEducation: TTabSheet
      Caption = #1054#1073#1088#1072#1079#1086#1074#1072#1085#1080#1077
      ImageIndex = 1
      object Labeducation: TLabel
        Left = 8
        Top = 8
        Width = 68
        Height = 13
        Caption = #1054#1073#1088#1072#1079#1086#1074#1072#1085#1080#1077
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labedudoc: TLabel
        Left = 8
        Top = 56
        Width = 135
        Height = 13
        Caption = #1044#1086#1082#1091#1084#1077#1085#1090' '#1086#1073' '#1086#1073#1088#1072#1079#1086#1074#1072#1085#1080#1080
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labeduplace: TLabel
        Left = 8
        Top = 104
        Width = 181
        Height = 13
        Caption = #1053#1072#1080#1084#1077#1085#1086#1074#1072#1085#1080#1077' '#1091#1095#1077#1073#1085#1086#1075#1086' '#1079#1072#1074#1077#1076#1077#1085#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Lablanguage: TLabel
        Left = 224
        Top = 152
        Width = 28
        Height = 13
        Caption = #1071#1079#1099#1082
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labeduenddate: TLabel
        Left = 8
        Top = 152
        Width = 74
        Height = 13
        Caption = #1043#1086#1076' '#1086#1082#1086#1085#1095#1072#1085#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labedudocinfo: TLabel
        Left = 224
        Top = 56
        Width = 132
        Height = 13
        Caption = #1057#1077#1088#1080#1103' '#1080' '#1085#1086#1084#1077#1088' '#1076#1086#1082#1091#1084#1077#1085#1090#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labeduspec: TLabel
        Left = 224
        Top = 8
        Width = 78
        Height = 13
        Caption = #1057#1087#1077#1094#1080#1072#1083#1100#1085#1086#1089#1090#1100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object ComboBoxeducation: TComboBox
        Left = 8
        Top = 24
        Width = 209
        Height = 21
        Hint = #1054#1073#1088#1072#1079#1086#1074#1072#1085#1080#1077
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnChange = ComboBoxeduformChange
      end
      object Editeduplace: TEdit
        Left = 8
        Top = 120
        Width = 489
        Height = 21
        Hint = #1059#1095#1077#1073#1085#1086#1077' '#1079#1072#1074#1077#1076#1077#1085#1080#1077
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        Text = #1053#1053#1043#1059
      end
      object ComboBoxlanguage: TComboBox
        Left = 224
        Top = 168
        Width = 273
        Height = 21
        Hint = #1071#1079#1099#1082', '#1082#1086#1090#1086#1088#1099#1084' '#1074#1083#1072#1076#1077#1077#1090' '#1089#1090#1091#1076#1077#1085#1090
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        OnExit = EditemailExit
      end
      object ComboBoxedudoc: TComboBox
        Left = 8
        Top = 72
        Width = 209
        Height = 21
        Hint = #1044#1086#1082#1091#1084#1077#1085#1090' '#1086#1073' '#1086#1073#1088#1072#1079#1086#1074#1072#1085#1080#1080
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnChange = ComboBoxeduformChange
      end
      object MaskEditeduenddate: TMaskEdit
        Left = 8
        Top = 168
        Width = 209
        Height = 21
        Hint = #1044#1072#1090#1072' '#1086#1082#1086#1085#1095#1072#1085#1080#1103' '#1091#1095#1077#1073#1085#1086#1075#1086' '#1079#1072#1074#1077#1076#1077#1085#1080#1103
        Color = clSilver
        EditMask = '!9999;1;_'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        MaxLength = 4
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        Text = '    '
      end
      object Editedudocinfo: TEdit
        Left = 224
        Top = 72
        Width = 273
        Height = 21
        Hint = #1053#1086#1084#1077#1088' '#1076#1086#1082#1091#1084#1077#1085#1090#1072' '#1086#1073' '#1086#1073#1088#1072#1079#1086#1074#1072#1085#1080#1080
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
      end
      object Editeduspec: TEdit
        Left = 224
        Top = 24
        Width = 273
        Height = 21
        Hint = #1057#1087#1077#1094#1080#1072#1083#1100#1085#1086#1089#1090#1100
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        Text = 'programmer'
      end
    end
    object TabSheetStudy: TTabSheet
      Caption = #1054#1073#1091#1095#1077#1085#1080#1077
      ImageIndex = 2
      object Labenternum: TLabel
        Left = 128
        Top = 164
        Width = 34
        Height = 13
        Caption = #1053#1086#1084#1077#1088
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labznum: TLabel
        Left = 8
        Top = 108
        Width = 101
        Height = 13
        Caption = #8470' '#1079#1072#1095#1077#1090#1085#1086#1081' '#1082#1085#1080#1078#1082#1080
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labgrp: TLabel
        Left = 8
        Top = 12
        Width = 35
        Height = 13
        Caption = #1043#1088#1091#1087#1087#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labcity: TLabel
        Left = 128
        Top = 108
        Width = 30
        Height = 13
        Caption = #1043#1086#1088#1086#1076
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labspec: TLabel
        Left = 128
        Top = 12
        Width = 78
        Height = 13
        Caption = #1057#1087#1077#1094#1080#1072#1083#1100#1085#1086#1089#1090#1100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labdognum: TLabel
        Left = 392
        Top = 204
        Width = 84
        Height = 13
        Caption = #1053#1086#1084#1077#1088' '#1076#1086#1075#1086#1074#1086#1088#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labdogshifr: TLabel
        Left = 128
        Top = 204
        Width = 79
        Height = 13
        Caption = #1064#1080#1092#1088' '#1076#1086#1075#1086#1074#1086#1088#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labdogyear: TLabel
        Left = 320
        Top = 204
        Width = 18
        Height = 13
        Caption = #1043#1086#1076
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label1: TLabel
        Left = 320
        Top = 164
        Width = 26
        Height = 13
        Caption = #1044#1072#1090#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labzach: TLabel
        Left = 8
        Top = 164
        Width = 112
        Height = 13
        Caption = #1055#1088#1080#1082#1072#1079' '#1086' '#1079#1072#1095#1080#1089#1083#1077#1085#1080#1080':'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labeduform: TLabel
        Left = 8
        Top = 61
        Width = 86
        Height = 13
        Caption = #1060#1086#1088#1084#1072' '#1086#1073#1091#1095#1077#1085#1080#1103
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Bevel4: TBevel
        Left = 8
        Top = 155
        Width = 489
        Height = 3
      end
      object Editenternum: TEdit
        Left = 128
        Top = 180
        Width = 185
        Height = 21
        Hint = #1053#1086#1084#1077#1088' '#1087#1088#1080#1082#1072#1079#1072' '#1086' '#1079#1072#1095#1080#1089#1083#1077#1085#1080#1080
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        Text = '21435'
      end
      object Editznum: TEdit
        Left = 8
        Top = 124
        Width = 113
        Height = 21
        Hint = #8470' '#1079#1072#1095#1077#1090#1082#1080
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 3
        Text = 'sdf'
      end
      object ComboBoxgrp: TComboBox
        Left = 8
        Top = 28
        Width = 113
        Height = 21
        Hint = #1042#1099#1073#1086#1088' '#1075#1088#1091#1087#1087#1099
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
        OnChange = ComboBoxgrpChange
        Items.Strings = (
          'weqw'
          'qweqwe'
          'qweqwe'
          'qwe')
      end
      object ComboBoxcity: TComboBox
        Left = 128
        Top = 124
        Width = 369
        Height = 21
        Hint = #1042#1099#1073#1086#1088' '#1075#1086#1088#1086#1076#1072
        Style = csDropDownList
        Color = clSilver
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 4
        OnChange = ComboBoxeduformChange
      end
      object ComboBoxspec: TComboBox
        Left = 128
        Top = 28
        Width = 369
        Height = 21
        Hint = #1042#1099#1073#1086#1088' '#1089#1087#1077#1094#1080#1072#1083#1100#1085#1086#1089#1090#1080
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 1
        OnChange = ComboBoxeduformChange
      end
      object Editdognum: TEdit
        Left = 392
        Top = 220
        Width = 105
        Height = 21
        Hint = #1053#1086#1084#1077#1088' '#1076#1086#1075#1086#1074#1086#1088#1072
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 10
        Text = '45345'
      end
      object ComboBoxdogyear: TComboBox
        Left = 320
        Top = 220
        Width = 65
        Height = 21
        Hint = #1043#1086#1076' '#1087#1086#1076#1087#1080#1089#1072#1085#1080#1103' '#1076#1086#1075#1086#1074#1086#1088#1072
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 9
      end
      object ComboBoxdogshifr: TComboBox
        Left = 128
        Top = 220
        Width = 129
        Height = 21
        Hint = #1042#1099#1073#1086#1088' '#1096#1080#1092#1088#1072' '#1076#1086#1075#1086#1074#1086#1088#1072
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 7
      end
      object MaskEditenterdate: TMaskEdit
        Left = 320
        Top = 180
        Width = 177
        Height = 21
        Hint = #1044#1072#1090#1072' '#1085#1072#1087#1080#1089#1072#1085#1080#1103' '#1087#1088#1080#1082#1072#1079#1072' '#1086' '#1079#1072#1095#1080#1089#1083#1077#1085#1080#1080
        Color = clSilver
        EditMask = '!99/99/9999;1;_'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        MaxLength = 10
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        Text = '  .  .    '
        OnExit = MaskEditbdateExit
      end
      object ComboBoxeduform: TComboBox
        Left = 8
        Top = 76
        Width = 489
        Height = 21
        Hint = #1060#1086#1088#1084#1072' '#1086#1073#1091#1095#1077#1085#1080#1103
        Style = csDropDownList
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnChange = ComboBoxeduformChange
      end
      object ComboBoxdogfast: TComboBox
        Left = 256
        Top = 220
        Width = 57
        Height = 21
        Style = csDropDownList
        Color = clSilver
        ItemHeight = 13
        TabOrder = 8
      end
    end
    object TabSheetRelease: TTabSheet
      Caption = #1042#1099#1087#1091#1089#1082
      ImageIndex = 3
      object LabOtch: TLabel
        Left = 8
        Top = 12
        Width = 117
        Height = 13
        Caption = #1055#1088#1080#1082#1072#1079' '#1086#1073' '#1086#1090#1095#1080#1089#1083#1077#1085#1080#1080':'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labexitnum: TLabel
        Left = 128
        Top = 13
        Width = 34
        Height = 13
        Caption = #1053#1086#1084#1077#1088
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 320
        Top = 13
        Width = 26
        Height = 13
        Caption = #1044#1072#1090#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Labmisc: TLabel
        Left = 10
        Top = 61
        Width = 63
        Height = 13
        Caption = #1055#1088#1080#1084#1077#1095#1072#1085#1080#1077
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Bevel6: TBevel
        Left = 8
        Top = 137
        Width = 489
        Height = 3
      end
      object Label7: TLabel
        Left = 8
        Top = 148
        Width = 55
        Height = 13
        Caption = #1044#1080#1087#1083#1086#1084' '#8470
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 8
        Top = 196
        Width = 126
        Height = 13
        Caption = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1086#1085#1085#1099#1081' '#1085#1086#1084#1077#1088
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label9: TLabel
        Left = 264
        Top = 148
        Width = 113
        Height = 13
        Caption = #1044#1072#1090#1072' '#1074#1099#1076#1072#1095#1080' '#1076#1080#1087#1083#1086#1084#1072
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label3: TLabel
        Left = 263
        Top = 196
        Width = 173
        Height = 13
        Caption = #1044#1072#1090#1072' '#1087#1088#1080#1089#1091#1078#1076#1077#1085#1080#1103' '#1082#1074#1072#1083#1080#1092#1080#1082#1072#1094#1080#1080
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 265
        Top = 247
        Width = 113
        Height = 13
        Caption = #1053#1086#1084#1077#1088' '#1087#1088#1086#1090#1086#1082#1086#1083#1072' '#1043#1040#1050
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Editexitnum: TEdit
        Left = 128
        Top = 32
        Width = 177
        Height = 21
        Hint = #1053#1086#1084#1077#1088' '#1087#1088#1080#1082#1072#1079#1072' '#1086#1073' '#1086#1090#1095#1080#1089#1083#1077#1085#1080#1080
        Color = clSilver
        TabOrder = 0
      end
      object MaskEditexitdate: TMaskEdit
        Left = 320
        Top = 32
        Width = 177
        Height = 21
        Hint = #1044#1072#1090#1072' '#1085#1072#1087#1080#1089#1072#1085#1080#1103' '#1087#1088#1080#1082#1072#1079#1072' '#1086#1073' '#1086#1090#1095#1080#1089#1083#1077#1085#1080#1080
        Color = clSilver
        EditMask = '!99/99/9999;1;_'
        MaxLength = 10
        TabOrder = 1
        Text = '  .  .    '
        OnExit = MaskEditbdateExit
      end
      object Memomisc: TMemo
        Left = 8
        Top = 80
        Width = 489
        Height = 49
        Hint = #1053#1077#1086#1073#1093#1086#1076#1080#1084#1099#1077' '#1087#1088#1080#1084#1077#1095#1072#1085#1080#1103
        Color = clSilver
        Lines.Strings = (
          'Memomisc')
        ScrollBars = ssVertical
        TabOrder = 2
      end
      object MaskEditedunumdiplom: TMaskEdit
        Left = 8
        Top = 168
        Width = 241
        Height = 21
        Hint = #1053#1086#1084#1077#1088' '#1076#1080#1087#1083#1086#1084#1072
        Color = clSilver
        EditMask = 'LLL 0000000;1;_'
        MaxLength = 11
        TabOrder = 3
        Text = '           '
      end
      object MaskEditedunumreg: TMaskEdit
        Left = 8
        Top = 216
        Width = 241
        Height = 21
        Hint = #1056#1077#1075#1080#1089#1090#1088#1072#1094#1080#1086#1085#1085#1099#1081' '#1085#1086#1084#1077#1088
        Color = clSilver
        MaxLength = 5
        TabOrder = 4
      end
      object MaskEditedudatediplom: TMaskEdit
        Left = 264
        Top = 168
        Width = 225
        Height = 21
        Hint = #1044#1072#1090#1072' '#1074#1099#1076#1072#1095#1080' '#1076#1080#1087#1083#1086#1084#1072
        Color = clSilver
        EditMask = '!99/99/9999;1;_'
        MaxLength = 10
        TabOrder = 5
        Text = '  .  .    '
        OnExit = MaskEditbdateExit
      end
      object MaskEditedudatequalif: TMaskEdit
        Left = 264
        Top = 216
        Width = 225
        Height = 21
        Hint = #1044#1072#1090#1072' '#1087#1088#1080#1089#1091#1078#1076#1077#1085#1080#1103' '#1082#1074#1072#1083#1080#1092#1080#1082#1072#1094#1080#1080
        Color = clSilver
        EditMask = '!99/99/9999;1;_'
        MaxLength = 10
        TabOrder = 6
        Text = '  .  .    '
        OnExit = MaskEditbdateExit
      end
      object CheckBoxedudiplomotl: TCheckBox
        Left = 8
        Top = 256
        Width = 177
        Height = 17
        Hint = #1044#1080#1087#1083#1086#1084' '#1089' '#1086#1090#1083#1080#1095#1080#1077#1084
        Caption = #1044#1080#1087#1083#1086#1084' '#1089' '#1086#1090#1083#1080#1095#1080#1077#1084
        TabOrder = 7
      end
      object ButtonAutoEntering: TButton
        Left = 264
        Top = 304
        Width = 225
        Height = 25
        Caption = #1042#1074#1077#1089#1090#1080' '#1076#1072#1090#1099' '#1087#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
        TabOrder = 8
        OnClick = ButtonAutoEnteringClick
      end
      object ButtonSaveDafaultDate: TButton
        Left = 264
        Top = 344
        Width = 225
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1076#1072#1090#1099' '#1087#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
        TabOrder = 9
        OnClick = ButtonSaveDafaultDateClick
      end
      object MaskEditedunumprotgak: TMaskEdit
        Left = 264
        Top = 265
        Width = 225
        Height = 21
        Hint = #1044#1072#1090#1072' '#1087#1088#1080#1089#1091#1078#1076#1077#1085#1080#1103' '#1082#1074#1072#1083#1080#1092#1080#1082#1072#1094#1080#1080
        Color = clSilver
        TabOrder = 10
      end
    end
  end
  object UpDownStudent: TUpDown
    Left = 480
    Top = 1
    Width = 25
    Height = 16
    Hint = #1055#1077#1088#1077#1093#1086#1076' '#1082' '#1089#1083#1077#1076#1091#1102#1097#1077#1081'/'#1087#1088#1077#1076#1099#1076#1091#1097#1077#1081' '#1079#1072#1087#1080#1089#1080' '#1089' '#1080#1079#1084#1077#1085#1077#1085#1080#1077#1084' '#1076#1072#1085#1085#1099#1093
    Anchors = [akTop, akRight]
    Min = 0
    Orientation = udHorizontal
    ParentShowHint = False
    Position = 0
    ShowHint = True
    TabOrder = 2
    Wrap = False
    OnClick = UpDownStudentClick
  end
end
