= データ活用してます？

== よくある光景。。。

上司：　WebAPサーバのレイテンシを測りたいんだよなー

先輩：　了解です！新人！Apacheのアクセスログを元に集計してみて！

新人：　へい( ﾟДﾟ)ゞﾋﾞｼｯ

先輩：　よろしくー

新人：　よしやるか！まずは、数十台のサーバからアクセスログを収集して。。Excelで集計して。。

。。。（数時間後）

新人：　つらたん。。Orz


こういった光景ってあると思うんですよね(筆者も昔経験したことがあります)
たしかにこの方法でも集計できるのですが、集計するのに大きな工数がかかるのと確認するまでに時間がかかってしまいます。

理想は、WebAPサーバで出力されたログをリアルタイムに取り込み、ブラウザからいつでも確認できたらいいですよね。

え、そんなことできるの？本当にできるの？
できるのです！
ログの取り込むための方法さえ知っていれば様々なログを可視化することができるのだーヽ(*ﾟдﾟ)ノ

ただ、ログを可視化するには、取り込むログを構造化し、取り込む必要があります。
そこで、ログを構造化し、取り込むためのログ収集管理ツールのLogstashを用いてお話ししていきます！
取り込んでしまえばあとは、好きなように可視化すればいいのです！


== Logstash
Logstashは、Elasticsearch社が提供するオープンソースログ収集管理ツールです。
様々なログをを同時に取り込み、変換し、取り込みます。
取り込み先としては、同社のElasticsearchと組み合わせることでニアリアルタイムにログを取り込むことができます。

LogstashのINPUTS・FILTERS・OUTPUTSの3層構造を以下の図で表すことができます。

//image[stage02-01][Logstashの構造#01]{
  Logstashの構造の図を追加
//}

//image[stage02-02][Logstashの構造#02]{
//}


=== INPUTS
データソースは、様々なデータ形式で多数のシステムに分散されてます。
Logstashを利用することで多様なInput形式に対応することができます。
ログや、メトリクス、Webアプリケーション、データストア、様々なクラウドサービスからのデータ投入を可能とします。


=== FILTERS
INPUTしたデータソースをLogstashのFilterで解析し、フィールドを識別し、構造化します。
データソースの変換には、"Grok Filter"やIPアドレスから地理情報を得るための"Geoip Filter"など様々な（フィルタライブラリ:@<href>{https://www.elastic.co/guide/en/logstash/current/filter-plugins.html}）が用意されてます。
くどいですが、このGrok Filterにフォーカスするします！


=== OUTPUTS
データを構造化したのち、Elasticsearchなどに出力します。
Elasticsearch以外にも出力先にすることが可能なため、環境に合わせた使い方ができます。


ここまででなんとなーくLogstashについてわかったですかね？
要は、インプットデータをLogstashに食べさせると、定義したフィルタを介して構造化し、出力先に指定したところに転送してくれるって感じですね。

それでは、次章では具体的にLogstashに触れていきたいと思います。
