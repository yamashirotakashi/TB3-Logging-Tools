= はじめに

りまりま団のもふもふ（Twitter:@froakie0021）です。
Nintendo Switchが欲しいのに、どこも在庫切れで泣いてます。
ゼルダの伝説 ブレス オブ ザ ワイルド（BotW）は待ちきれずにWiiU版をはじめました。
今まで風のタクト推しだったのですが、BotW推しに乗り換えることにします。


…というのはさておき、この本ではデータ収集ツールを題材としています。
データ収集ツールはfluentdやLogstashが有名ですが、実際のところ「何が違うのか説明できる？」と言われると
なかなか難しいものがあります。ミドルウェアの選定は難しいですね。


というわけで、この本の前半では有名なデータ収集ツールであるfluentdとLogstashの機能や性能を比較してみました。
後半戦はみっちー（Github:@micci184）さんがLogstashのGrok filter
（Logstashの設定項目の1つ）について熱く語ってます。

なんでも「おれはGrok filterの闇をさまよったが、マスターしてきた！」とのことです。
Logstashのconfigファイルは癖があるので、悩まれている方は考え方など参考になるかと思います。

こちらもどうぞよろしくお願いいたします。


「こんなのもあるよー」とかありましたら、Twitterとかで教えてもらえると嬉しいです。
あと、どーやったらNintendo Switchが買えるのかもついでに教えてもらえるともっと嬉しいです。

== 注意事項とか

この本に出てくる各ソフトウェアのバージョンは下記の通りです。
また、第1章から第6章まではCentOS6、それ以降はAmazon Linuxを使用して動作を確認しています。

* Logstash
** Version 6.0(Beta)
* Elasticsearch
** Version 6.0(Beta)
* fluentd
** Version 0.12(td-agent v2.3.5)

本の内容は公式ドキュメントや参考書籍などを元に動作確認を行ってはいますが、
この本の情報を用いた開発・制作・運用に対して発生した全ての結果に対して責任は負うことはできません。
必ずご自身の環境でよく検証してから導入をお願いします。

もふもふ（@froakie0021）
