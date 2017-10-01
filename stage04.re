= アクセスログをいい感じに取り込んでみよう
== ログを取り込むまでのステップ
ログをよしなに取り込むにはFILTERでログフォーマットに合わせて定義をする必要があります。
なので、この章ではどのようにログを取り込むかをステップを踏んで解説していきたいと思います。

1. 取り込むログフォーマットを調べる
2. Grokのお時間
3. Grok Constructorでテスト
4. logstashのconfファイルで動かしてみる

結構、ステップ面倒いなーって思う人もいるかと思いますが、一つ一つクリアしていくことが大切だと思ってます。
地味ーな作業が盛りだくさんですが、自分の思った通りにFILTERがかかった時が最高に嬉しい瞬間です！

それでは一つ一つ見ていきたいとおもいまするー

== 取り込むログフォーマットを調べる

今回取り込むログは、Apacheのログフォーマットがcommonですね。
Apacheのサイトからログフォーマットについて調べると色々と記載されてます。

Apacheのアクセスログのログフォーマットは以下な感じです。


* LogFormat "%h %l %u %t \"%r\" %>s %b" common
** %h: サーバへリクエストしたクライアントIP
** %l: クライアントのアイデンティティ情報ですが、デフォルト取得しない設定になっているため、”-”（ハイフン）で表示される
** %u: HTTP認証によるリクエストしたユーザID（認証していない場合は、"-"）
** %t: サーバがリクエストを受け取った時刻
** \"%r\": メソッド、パス、プロトコルなど
** %>s: ステータスコード
** %b: クライアントに送信されたオブジェクトサイズ（送れなかった時は、"-"）

== フィールド定義していくよ

アクセスログのログフォーマットがわかったので、フィールド名を定義していきたいと思いますー


* %hは、クライアントIPということで"clientip"
* %lは、アイデンティティ情報なので、"ident"
* %uは、認証なので、"auth"
* %tは、時刻なので"date"
* \"%r\"は、いくつかに分割したいので、メソッドは、"method"、パスは、"path"、んでHTTPバージョンは、"httpversion"
* %>sは、ステータスコードなので、"response"
* %bは、オブジェクトサイズなので、"bytes"


仮に上記がマッピングされると以下のようにいい感じになるはず！


* 5.10.83.30 - - [10/Oct/2000:13:55:36 -0700] "GET /test.html HTTP/1.0" 200 2326
** clientip: 5.10.83.30
** ident: -
** auth: -
** date: 10/Oct/2000:13:55:36 -0700
** method: GET
** path: /test.html
** httpversion: 1.0
** response: 200
** bytes: 2326

== GrokPattern

Grokは、様々なログを正規表現を駆使していい感じにフィールド分割して、マッチさせるためのプラグインです。
Grokは、GrokPatternという形であらかじめ正規表現のパターン定義が用意されているので、ふんだんに使っていきたいと思います。
ただ、GrokPatternにないものは自分で作成する必要がありますー
そこも含めて解説できればと思いますm(_ _)m

(GrokPattern)[https://github.com/elastic/logstash/blob/v1.4.2/patterns/grok-patterns]

それでは、ここからはフィールド一つ一つを見ていってGrokPatternを作成していきたいと思います。
GrokPatternを作成していくには、ログの左から順に攻略していくのが重要です。
ということを念頭において進めていきたいと思います。

ちなみにですが、そもそものGrokFilterの書き方とかはひとまず置いておきます！
後ほど、その辺は詳しく書きます。

=== ClientIP
ClientIPといことで、IPアドレスにマッチさせる必要があります。
まずは、IPアドレスにマッチさせるためのGrokPatternがすでにないかを先ほどのGrokPatternのサイトから確認します。

・・・あるではないか！（茶番劇っぽくてすまそんです）


* IPORHOST (?:%{HOSTNAME}|%{IP})

IPORHOST内に%{HOSTNAME}と%{IP}で構成されており、別に%{HOSTNAME}と%{IP}がGrokPatternとして定義されているので、それらを読み込む様になってます。
また、先ほどGrokPatternサイトで調べてみると...ありますね！


* HOSTNAME \b(?:[0-9A-Za-z][0-9A-Za-z-]{0,62})(?:\.(?:[0-9A-Za-z][0-9A-Za-z-]{0,62}))*(\.?|\b)
* IP (?:%{IPV6}|%{IPV4})

HOSTNAMEに正規表現が記載されていることがわりますね。
また、IPは、IPv6とIPv4に対応できるように構成されてます。
これも同じ様にサイトをみると正規表現で記載されていることがわかると思います。

IPORHOSTでHOSTNAMEとIPが定義されていることがわかったと思いますが、(?:)と|（パイプ）はなんぞや？と思った人もいると思いますが、
この(?:)は、文字列をマッチさせたい&&キャプチャさせたくない場合に使います（キャプチャは使用しないので説明しません）
今回でいう文字列は、%{HOSTNAME}と%{IP}に該当する文字列を指します。
また、|は、どちからが一致したらを意味します。

結果、IPORHOSTは、HOSTNAMEかつ、IPに該当するものをマッチさせます。

上記を踏まえてGrokPatternを記載すると以下な感じになります。

* %{IPORHOST:clientip}

これらを図にすると以下です。

//image[grok01][IPアドレスをGrokするイメージ図#01][scale=0.5]{
  Grokパワポ
//}

それでは、実際にGrokがマッチされるかをGrok Constructorを使って確認してみたいと思います。

== Grok Constructor
(Grok Constructor)[http://grokconstructor.appspot.com/do/match]は、作成したGrokがマッチするかをブラウザベースでテストすることが可能なツールです。
この他にも(GrokDebugger)[https://grokdebug.herokuapp.com/]やKibanaのDevToolで提供しているGrokDebuggerなどがあります。

Grok Constructorの使い方を以下の図に記載します。

//image[grok_constructor01][Grok Constructorでテスト#01][scale=0.5]{
  Grokパワポ
//}

それでは、早速先ほど作成したGrokPatternがうまい具合にマッチするか試したいと思います。

//image[grok_constructor02][Grok Constructorでテスト#02][scale=0.5]{
  Grokパワポ
//}

想定通り、clientipというフィールドに "5.10.83.30"というIPアドレスがマッチしたことがわかります。
この調子で、他のフィールドに対しても定義していきたいと思います！

=== ident
ユーザ名が付与入ってくるのと"-"がマッチできるものをGrokPatternで見ていくと、USERというGrokPatternがあるのでこちらを使用します。

* %{USER:ident}

先ほどの様に、上記のGrokPatternでGrok Constructorでテストを実施するとIPアドレスが引っかかると思います。
なので、%{IPORHOST:clientip}を含んでテストを実施してみてください。

//image[grok_constructor03][Grok Constructorでテスト#03][scale=0.5]{
  Grokパワポ
//}

=== auth
authもUserと同様の定義で良いので、GrokPatternのUSERを使用します。
また、identとauthの間もスペースがあるので\sもしくはスペースを入力する必要があります。
 ※図の記載では\sを¥sで記載してますm(_ _)m

=== date

次は、時刻ですね！
時刻のフォーマットは、"[day/month/year:hour:minute:second zone]"です。
これに当てはまるGrokPatternを探していたいと思いますー

結果、以下のGrokPatternが当てはまることがわかります。

* HTTPDATE %{MONTHDAY}/%{MONTH}/%{YEAR}:%{TIME} %{INT}

なので、こちらを使用してGrok Constructorでテストしてみたいと思います。
先ほど作成したGrok Constructorに連ねてきましょー

//image[grok_constructor04][Grok Constructorでテスト#04][scale=0.5]{
  Grokパワポ
//}

あれ？"NOT MATCHED"ですね。。
そうなんです！
じつは、%{HTTPDATE}に該当しない[]があるのです。
なので、以下の図で示している通り、[]を取り除く必要があるのですー
無効化するにはエスケープ\（バックスラッシュ）を使用します。

//image[grok02][IPアドレスをGrokするイメージ図#02][scale=0.5]{
  Grokパワポ
//}

=== リクエスト
それでは、クライアントからのリクエストについて攻略したいと思いますー
これは、ダブルクォーテーションの中にひとまとまりされているので、取りたい情報を定義したフィールドにマッチできるようにGrokPatternを作成していきたいと思います。

* "GET /test.html HTTP/1.0"

まず、GETですが、GETという文字列以外にもPOSTや、DELETEなどあります。
なので単純にGETという固定文字でマッチングさせるのでは、あかんのです。
また、GET|PUT|DELETE..etcなどもイケてないですね。。

ということで、英単語が入るということがわかっているので、\bxxx\bに該当するGrokPatternを使用します。
 ※xxx：何かしらの英単語の意味

 いつも通り、GrokPatternを見ると以下が該当しますね。

 * WORD \b\w+\b

次にパスですが、リクエストによって変動したりするため、柔軟性を求めて以下のNOTSPACEを使用します。
NOTSPACEは、空白文字以外にマッチのため、空白文字が出現するまでマッチします。

* NOTSPACE \S+

最後のHTTPのバージョンですが、HTTP部分は不要なので取り除くのと、そもそも、HTTPバージョンがはいっていないパターンもあったりします。
そんな時は、(?:)?を利用するこで、このGrokPatternにマッチする時は使うけど、マッチしない時は使わないよ！といった定義ができるのですー
これは、便利なので覚えて置いてくださいな！
最後に最短マッチとして、%{DATA}もパイプで組み込んでます。

* (?: HTTP/%{NUMBER:httpversion})?|%{DATA:rawrequest})"


てことで、ここまでを以下の図にまとめましたーヽ(*ﾟдﾟ)ノ

//image[grok03][IPアドレスをGrokするイメージ図#03][scale=0.5]{
  Grokパワポ
//}

=== response & bytes

ここまできたらあと少し！
responseは、ステータスコードなので、NUMBERを使用します。
また、bytesも同様にNUMBERを使用しますが、オブジェクトが送れなかった場合は、"-"のため、|で"-"を追加します。

これで全て整ったので、Grok Constructorでテストしたいと思います。

== Grok Constructorで全体テスト
以下のGrokPatternでテストをしたいと思います。

=== 完成したGrokPattern

* %{IPORHOST:clientip} %{USER:ident} %{USER:auth} \[%{HTTPDATE:date}\] "(?:%{WORD:method} %{NOTSPACE:path}(?: HTTP/%{NUMBER:httpversion})?|%{DATA:rawrequest})" %{NUMBER:response} (?:%{NUMBER:bytes}|-)

== Logstashのconfファイルの作成

やっとここでLogstashのconfファイルが登場しますw
長かったですねw
てことで、confファイルを作成したいと思いますー

今までは、INPUTとOUTPUTのみでしたが、先ほど作成したGrokPatternを埋め込みたいので、FILTERを追加します。
そこにGrokPatternを直接、Logstashのconfファイルにコーディングするのも可能ですが、可読性を意識したいため、GrokPatternのファイルを外出しします。

外出しするため、以下の作業を実施します。

//cmd{
### GrokPatternファイルを配置するためのディレクトリを作成
$ mkdir patterns
### httpd用のGrokPatternファイルを作成
### GrokPattern名をHTTPD_COMMONLOGとします
$ vim patterns/httpd_Patterns
HTTPD_COMMONLOG %{IPORHOST:clientip} %{USER:ident} %{USER:auth} \[%{HTTPDATE:date}\] "(?:%{WORD:method} %{NOTSPACE:path}(?: HTTP/%{NUMBER:httpversion})?|%{DATA:rawrequest})" %{NUMBER:response} (?:%{NUMBER:bytes}|-)
}

次にGrokPatternファイルを作成したので、ログの変換をさせるためとGrokPatternを読み込むためにLogstashのconfに以下を記載します。

//cmd{
$ vim conf.d/test03.conf
input {
  file {
    path => "/etc/logstash/log/httpd_access.log"
    start_position => "beginning"
  }
}
filter {
  grok {
    patterns_dir => ["/etc/logstash/patterns/httpd_patterns"]
    match => { "message" => "%{HTTPD_COMMONLOG}" }
  }
output {
  stdout { codec => rubydebug }
}
}

それでは、実行してみますー

//cmd{
$ usr/share/logstash/bin/logstash -f conf.d/test02.conf
### 結果！！
{
        "request" => "/test.html",
           "auth" => "-",
          "ident" => "-",
           "verb" => "GET",
        "message" => "5.10.83.30 - - [10/Oct/2000:13:55:36 -0700] \"GET /test.html HTTP/1.0\" 200 2326",
           "path" => "/etc/logstash/log/httpd_access.log",
     "@timestamp" => 2017-10-01T15:11:19.695Z,
       "response" => "200",
          "bytes" => "2326",
       "clientip" => "5.10.83.30",
       "@version" => "1",
           "host" => "0.0.0.0",
    "httpversion" => "1.0",
      "timestamp" => "10/Oct/2000:13:55:36 -0700"
}
}

おぉ！いい感じにフィールドが抽出できたーヾ(´Д｀)ノｲｴｰｲ
が、しかし、コレでは足りない！
ログのタイムスタンプではなく、ログを取り込んだ時刻になっているので、修正が必要です。
また、グローバルIPがあるんだから、地域情報とマッピングしたいですよね！
ということで、Logstashのconfファイルを修正したいと思いますー

//cmd{
$ vim conf.d/test03.conf
input {
  file {
    path => "/etc/logstash/log/httpd_access.log"
    start_position => "beginning"
  }
}
filter {
  grok {
    match => { "message" => "%{HTTPD_COMMONLOG}" }
  }
  geoip {
    source => "clientip"
  }
  date {
    match => [ "date", "dd/MMM/YYYY:HH:mm:ss Z" ]
    locale => "en"
    target => "timestamp"
  }
  mutate {
    remove_field => [ "message", "path", "host" ]
  }
}
output {
  stdout { codec => rubydebug }
}

各々のフィルターについて図で説明します。

//image[grok03][confファイルのFilterについて][scale=0.5]{
  Grokパワポ
//}


それでは、修正したconfファイルで再度実行すると以下の感じになります。
地理情報やタイムスタンプや不要な情報が削除されていることがわかります。

//cmd{
{
        "request" => "/test.html",
          "geoip" => {
              "timezone" => "Europe/Amsterdam",
                    "ip" => "5.10.83.30",
              "latitude" => 52.35,
        "continent_code" => "EU",
             "city_name" => "Amsterdam",
          "country_name" => "Netherlands",
         "country_code2" => "NL",
         "country_code3" => "NL",
           "region_name" => "North Holland",
              "location" => {
            "lon" => 4.9167,
            "lat" => 52.35
        },
           "postal_code" => "1091",
           "region_code" => "NH",
             "longitude" => 4.9167
    },
           "auth" => "-",
          "ident" => "-",
           "verb" => "GET",
     "@timestamp" => 2017-10-01T15:44:25.815Z,
       "response" => "200",
          "bytes" => "2326",
       "clientip" => "5.10.83.30",
       "@version" => "1",
    "httpversion" => "1.0",
      "timestamp" => "10/Oct/2000:13:55:36 -0700"
}
}


如何でしたか？
これでApacheログをGrokを利用して抽出できるようになったのではないでしょうか？
次は、いままで使ってきたGrokPatternなどを利用してAWSサービスのログを抽出したいと思います。
それでは、次章でーヽ(*ﾟдﾟ)ノ
