# CHANGELOG-チェンジログ

## Active Version
**[1.1.2]**

---
## [1.1.2] - 2025-08-27

### changed
- ASerialCoreを`1.1.0`に更新
- ASerialCore更新に伴い、WriteDataのパケットバッファの動作を修正

## [1.1.1] - 2025-08-26

### changed
- ASerialCoreを`1.0.1`に更新
- ReadDataProcessのシリアル読み取りをuint8_tに変更

## [1.1.0] - 2025-08-20

### added
- ReadDataProcessを内部完結し、タイムアウト検出ができる`ReadData`を追加

**1.1.0にした際にチェンジログを書くのを忘れていたので1.1.1のタイミングで追加しました**

## [1.0.1] - 2025-08-10

### Fixed
- Nullバイトが破棄されてしまう問題を修正

### added
- ConnectDevice時に接続されたときにバッファをクリアするように修正

### Changed
- dcb.fNull = TRUE => FALSE;    // NULLバイトの破棄の設定の変更
- 変数をすべて初期化するように修正
- 自動接続テストサンプルをデバイスIDを指定して接続するように変更

## [1.0.0] - 2025-07-22

### Added
- チェンジログの記録を開始
- MITライセンスの追加
- 新バージョンのASerialCoreを使用

### Removed
- 旧バージョンのASerial_coreを削除
- 不要なディレクトリの削除

---



## [フォーマットルール]

- 各バージョンは SemVer（MAJOR.MINOR.PATCH）で管理
- 日付は ISO形式 `YYYY-MM-DD` で記載
- セクション分類の例：`Added`, `Changed`, `Fixed`, `Removed`, `Deprecated`, `Security`
