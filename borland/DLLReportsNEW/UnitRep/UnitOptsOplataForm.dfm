object FormOptsOplata: TFormOptsOplata
  Left = 178
  Top = 199
  Width = 685
  Height = 428
  Anchors = [akLeft, akTop, akRight, akBottom]
  Caption = #1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1086#1087#1083#1072#1090#1099
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    677
    394)
  PixelsPerInch = 96
  TextHeight = 13
  object BtnSelectClose: TSpeedButton
    Left = 572
    Top = 357
    Width = 105
    Height = 22
    Hint = #1042#1099#1073#1088#1072#1090#1100' '#1085#1091#1078#1085#1091#1102' '#1082#1072#1090#1077#1075#1086#1088#1080#1102' '#1086#1087#1083#1072#1090#1099' '#1076#1083#1103' '#1075#1077#1085#1077#1088#1072#1094#1080#1080' '#1086#1090#1095#1077#1090#1072
    Anchors = [akRight, akBottom]
    Caption = '&'#1054#1090#1095#1077#1090
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
    ParentShowHint = False
    ShowHint = True
    OnClick = BtnSelectCloseClick
  end
  object BtnAdd: TSpeedButton
    Left = 648
    Top = 184
    Width = 23
    Height = 22
    Hint = #1044#1086#1073#1072#1072#1074#1080#1090#1100' '#1082#1072#1090#1077#1075#1086#1088#1080#1102
    Anchors = [akTop, akRight]
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333393333
      333333333337F3333333333333397333333333333337FF333333333333C94333
      3333333333737F333333333333C9473333333333337373F3333333333CC94433
      3333333337F7F7F3333333333CC94473333333333737F73F33333333CCC94443
      333333337F37F37F33333333CCC94447333333337337F373F333333CCCC94444
      33333337F337F337F333333CCCC94444733333373337F3373F3333CCCCC94444
      4333337F3337FF337F3333CCCCC94444473333733F7773FF73F33CCCCC393444
      443337F37737F773F7F33CCC33393374447337F73337F33737FFCCC333393333
      444377733337F333777FC3333339333337437333333733333373}
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = True
    OnClick = BtnAddClick
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 375
    Width = 677
    Height = 19
    Hint = #1044#1072#1085#1085#1099#1077' '#1074#1099#1073#1088#1072#1085#1085#1086#1081' '#1082#1072#1090#1077#1075#1086#1088#1080#1080
    Panels = <
      item
        Bevel = pbNone
        Text = #1043#1088#1091#1087#1087#1072
        Width = 45
      end
      item
        Width = 80
      end
      item
        Width = 5
      end
      item
        Bevel = pbNone
        Text = #1055#1077#1088#1080#1086#1076' '#1086#1087#1083#1072#1090#1099
        Width = 90
      end
      item
        Width = 230
      end
      item
        Width = 5
      end
      item
        Bevel = pbNone
        Text = #1057#1091#1084#1084#1072
        Width = 40
      end
      item
        Width = 70
      end
      item
        Bevel = pbNone
        Width = 50
      end>
    ParentShowHint = False
    ShowHint = True
    SimplePanel = False
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 677
    Height = 177
    Align = alTop
    Caption = ' '#1057#1091#1097#1077#1089#1090#1074#1091#1102#1097#1080#1077' '#1082#1072#1090#1077#1075#1086#1088#1080#1080' '#1076#1083#1103' '#1089#1090#1091#1076#1077#1085#1090#1086#1074' '#1074#1099#1073#1088#1072#1085#1085#1086#1081' '#1075#1088#1091#1087#1087#1099' '
    TabOrder = 1
    object StrGridOpts: TStringGrid
      Left = 2
      Top = 15
      Width = 673
      Height = 160
      Hint = 
        #1058#1072#1073#1083#1080#1094#1072' '#1082#1072#1090#1077#1075#1086#1088#1080#1081' '#1086#1087#1083#1072#1090#1099' '#1079#1072' '#1086#1073#1091#1095#1077#1085#1080#1077' ('#1076#1074#1086#1081#1085#1086#1081' '#1097#1077#1083#1095#1086#1082' '#1087#1086' '#1089#1090#1088#1086#1095#1082#1077' ' +
        #1072#1085#1072#1083#1086#1075#1080#1095#1077#1085' '#1074#1099#1073#1086#1088#1091' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1081' '#1079#1072#1087#1080#1089#1080')'
      Align = alClient
      BorderStyle = bsNone
      Color = clBtnFace
      ColCount = 4
      DefaultRowHeight = 16
      RowCount = 2
      GridLineWidth = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goColMoving, goRowSelect, goThumbTracking]
      ParentShowHint = False
      ScrollBars = ssVertical
      ShowHint = True
      TabOrder = 0
      OnClick = StrGridOptsClick
      OnDblClick = StrGridOptsDblClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 209
    Width = 677
    Height = 144
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = ' '#1050#1072#1090#1077#1075#1086#1088#1080#1080' '#1086#1090#1087#1083#1072#1090#1099', '#1074#1099#1073#1088#1072#1085#1085#1099#1077' '#1076#1083#1103' '#1075#1077#1085#1077#1088#1072#1094#1080#1080' '#1086#1090#1095#1077#1090#1072' '
    TabOrder = 2
    object StringGridRealOpts: TStringGrid
      Left = 2
      Top = 15
      Width = 673
      Height = 127
      Hint = 
        #1058#1072#1073#1083#1080#1094#1072' '#1082#1072#1090#1077#1075#1086#1088#1080#1081' '#1086#1087#1083#1072#1090#1099' '#1079#1072' '#1086#1073#1091#1095#1077#1085#1080#1077' ('#1076#1074#1086#1081#1085#1086#1081' '#1097#1077#1083#1095#1086#1082' '#1087#1086' '#1089#1090#1088#1086#1095#1082#1077' ' +
        #1072#1085#1072#1083#1086#1075#1080#1095#1077#1085' '#1074#1099#1073#1086#1088#1091' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1081' '#1079#1072#1087#1080#1089#1080')'
      Align = alClient
      BorderStyle = bsNone
      Color = clBtnFace
      ColCount = 4
      DefaultRowHeight = 16
      RowCount = 2
      GridLineWidth = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goColMoving, goRowSelect, goThumbTracking]
      ParentShowHint = False
      ScrollBars = ssVertical
      ShowHint = True
      TabOrder = 0
      OnClick = StrGridOptsClick
      OnDblClick = StrGridOptsDblClick
    end
  end
  object ZMySqlDatabase: TZConnection
    Protocol = 'mysql-4.0'
    Port = 3306
    AutoCommit = True
    ReadOnly = True
    TransactIsolationLevel = tiNone
    Connected = False
    SQLHourGlass = False
    Left = 144
    Top = 104
  end
  object ZMySqlQuery: TZQuery
    Connection = ZMySqlDatabase
    RequestLive = False
    CachedUpdates = False
    ParamCheck = True
    Params = <>
    ShowRecordTypes = [utUnmodified, utModified, utInserted]
    UpdateMode = umUpdateChanged
    WhereMode = wmWhereKeyOnly
    Options = [doCalcDefaults]
    Left = 280
    Top = 120
  end
end
