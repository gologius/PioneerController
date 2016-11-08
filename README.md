# README #


## 概要##
MobileRobots社製 Pioneer 3DX をキーボードで操作できる簡易サンプルです．C++，VisualStudio

## 環境構築 ##
1. SDKをインストールする 
http://robots.mobilerobots.com/wiki/Main_Page

1. RS-232Cのコネクタにケーブルを挿す 

###確認方法###
* 下記マニュアルのChapter5を参考に，demoが動けばOK(demoは矢印キーで操作できる．遅延する？ので注意) 
http://www.inf.ufrgs.br/~prestes/Courses/Robotics/manual_pioneer.pdf

* bin or bin64 フォルダ内のdemo.exeを直接コマンドプロンプトで起動する．その際引数が必要．
以下は例．9600はデフォルト値だが，COM4は各自で調査，変更する必要あり 

```bash
>> demo.exe -robotPort COM4 -robotBaud 9600
```

## 実行 ##
上記の環境構築と動作確認を行ったうえで，ビルド，実行する
