# このForkについて

このForkは、当方がQiitaにて執筆中のシリーズ[【SPIKE-RTでロボコンに出よう!!】](https://qiita.com/koushiro/items/482162807339f4d18516)で紹介した開発環境構築に基づき、必要なファイルの新規作成、及び編集を行ったものである。

# 編集箇所

## 新規作成ファイル

新規作成ファイルは[#3 テンプレートファイルとシェルスクリプトの作成](https://qiita.com/koushiro/items/3cf4cff56e0bd90cac7a)の記事に従って作成している。

- `workspace` フォルダ
  - `template` フォルダ
    - `template.c`
    - `template.h`
    - `template.cfg`
    - `template.cdl`
    - `Makefile.inc`
  - 4つのサンプルプロジェクト
- `new_scripts` フォルダ
  - `build.sh`
  - `create.py`
  - `new_project.sh`
  - `start.sh`
  - `write.sh`

## 編集ファイル

編集ファイルは、デバッグ用に使用されているFポートをモータやセンサ等に使用できるようにするための編集を行っており、[【番外編】ポートFを使えるようする](https://qiita.com/koushiro/private/480559806f5572902c5c)の記事に従って作成している。

- `/spike-rt/external/libpybricks/lib/pbio/include/pbdrv/config.h`
- `/spike-rt/external/libpybricks/lib/pbio/platform/prime_hub_spike-rt/pbdrvconfig.h`
- `/spike-rt/external/libpybricks/lib/pbio/platform/prime_hub_spike-rt/pbioconfig.h`
- `/spike-rt/asp3/target/primehub_gcc/primehub.h`
- `/spike-rt/drivers/pybricks.cfg`

# サンプルプロジェクト

本Forkにて新たに作成した`workspace`フォルダ内には、当方で作成したサンプルプロジェクトが含まれている。
サンプルプロジェクトの中身について簡単に記す。

## 2file_test
ソースファイルを増やすサンプル。
Makefile.inc に新しく追加した「function.c」を読み込む設定を書いている。
(NEWFILE_COBJS = functions.o)

[学習事項]
- ファイルの追加 

## gyro_test

ハブブロックのジャイロセンサを使用するサンプル。
サイクルハンドラ(周期ハンドラ)を使用して、ジャイロセンサの加速度情報から角度を1msごとに算出している。
又、USB経由でシリアル通信を行い、PC側のシリアルモニタで現在の角度が監視できる。

[学習事項]
- ジャイロセンサ
- 周期ハンドラ
- シリアル通信

## gyro_turn

gyro_testの応用で、算出出来るようになった角度を用いて、90度回転をするもの。

[学習事項]
- ジャイロセンサ
- 周期ハンドラ

## multi_task_test3

マルチタスク、およびサイクルハンドラ(周期ハンドラ)のサンプル。
最初の5秒間はメインタスク(ハブディスプレイのカウントアップ)のみが行われ、次の5秒間はメインに加えてサブタスク(ボタンが赤色に点滅)が実行される。
さらに次の5秒間はメイン・サブに加えてサイクルハンドラにより実行されるタスク(スピーカーから音を鳴らす)が行われる。
3つの独立した動きが、同時に実行される様子が確認できる。

[学習事項]
- マルチタスク
- 周期ハンドラ


## シリアルモニタについて

シリアル通信を使う場合は、PC側に「シリアルモニタ」と呼ばれるソフトが必要である。
基本的には、自分の好きなものを使うと良い。
新たに入れる場合は、古くから使われている有名なソフトとして「Tera Term」をお勧めする。

https://github.com/TeraTermProject/osdn-download/releases

ここの「teraterm-5.0.exe」のようなインストーラーをダウンロード⇒インストールする。

もしくは、Arduino IDEに内蔵されている「シリアルモニタ」を使用しても良い。
「ツール」→「ポート」から適切なCOMポートを選べば、ハブブロックと通信することが出来る。
どのCOMポートかわからない場合は、いつも使用している「usbpid list」で調べると、
「STMicroelectronics Virtual COM Port (COM10)」
のように出現するはずである。
(決してこれをWSLにアタッチしないように。Windows側から接続できなくなる。)
