= 比較その2：コンフィグの形式
fluentdとLogstashのコンフィグ形式は、かなり違いがあります。
ここではデータの読み取り・加工・出力に分けて、コンフィグの比較をしてみたいと思います。
なお、今回のコンフィグ例ではTwitterから取得したjsonデータをcsvに加工する、というユースケースを想定しています。
Twitterから取得できるjsonの例をあげます@<fn>{example_twitter}。

//footnote[example_twitter][これはもふもふのTweetをいくつか取得したものです。大したことはつぶやいていませんね。]

//list[twitter_json][Twitterから取得したつぶやきデータの例]{
  jsonを挿入
//}

この章で扱う入力データの例は、全て@<list>{twitter_json}を使用しています。

== fluentd

=== コンフィグの構造
fluentdのコンフィグ（td-agentを用いてインストールした場合は@<code>{td-agnet.conf}）の構造は、次のようになっています。
また、コンフィグは@<code>{/etc/td-agent/td-agnet.conf}にデフォルトで配置されているので、こちらを編集していきます。

//image[fluentd_config][fluentdのコンフィグ構造][scale=0.3]{
//}

ただし、処理の順序は@<code>{source}→@<code>{filter}→@<code>{match}となっています。

=== データの読み取り部（source）
fluentdがどのデータを読み取るのかをこのディレクティブで指定します。データの入力元は複数指定することが可能です。
@<code>{<source>}と@<code>{</source>}の間に使用したいプラグインを指定していきます。
この書式は他のディレクティブでも同様です。


//list[fluentd_source][source部分の実装例]{
//}

=== データの加工部（filter）
読み取ったデータをどのように加工するのか指定するプラグインです。ただし、@<code>{filter}ディレクティブの中では
tagの変更ができないため、加工後のデータに新しくtagを付与したい場合は@<code>{match}ディレクティブで専用プラグインを使用します。

//list[fluentd_filter][filter部分の実装例]{
//}


=== データの送付部（match）
処理が終わったデータをどこに送付するか指定するディレクティブです。
プラグインを複数記述すれば、複数の出力先へデータを送付することが可能です。

@<list>{fluentd_match}

=== その他特徴など

== Logstash

=== コンフィグの構造
対するLogstashのコンフィグ（@<code>{logstash.conf}）の構造は、次のようになっています。
また、コンフィグは@<code>{/etc/logstash/conf.d}配下に置きます。名称の最後を@<code>{.conf}にして配置します。

//image[Logstash_config][Logstashのコンフィグ構造][scale=0.3]{
//}

特徴として、コンフィグにはデータ処理の内容しか記載しないこと・各データに対するタグ付けが任意であることが挙げられます。
Logstash自体のソフトウェアに関する動作設定は、全て@<code>{/etc}配下にある設定ファイル（@<code>{logstash.yml}）で行います。
ここでは@<code>{logstash.conf}のみに焦点を絞って中身をみていきます。

=== データの読み取り部（input）
Logstashがどのデータを読み取るのか、この部分で指定します。必須設定項目はプラグインごとに異なり、共通のものはありません。
取得したいデータに合わせてどのプラグインを使用するか決定し、使用方法を確認する必要があります。

今回はサーバー内に配置されたjsonファイルを取得するため、@<code>{file}プラグインを使用します（@<href>{https://www.elastic.co/guide/en/logstash/6.0/plugins-inputs-file.html}）。

//list[input_json_logstash][inputプラグインの実装例]{
input {
  file {
  path => "/var/log/json/*.json"
  }
}
//}

@<code>{file}はプラグイン名です。@<code>{path}はどこからデータを取得するか記載しています。この場合@<code>{/var/log/json}から
jsonファイルを全て取得する、という設定になっています。


=== データの加工部（filter）
読み取ったデータをどのように加工・整形するのか指定します@<fn>{logstash_filter}。

//footnote[logstash_filter][ここでの加工・整形とは、データの新規追加・削除・データ型の変更・データ分割などを指します。]

プラグインごとにできることが異なりますので、プラグイン同士を組み合わせてコンフィグを作成します。
基本は取得するデータ形式に応じたプラグインでデータをいい感じに分割し、その後データを煮るなり焼くなりして加工する、という感じです@<fn>{filter_make}。

//footnote[filter_make][こういうの、がらがらぽんってできると楽だと思うんですけど世の中厳しい。]

例えば、今回のようにjsonを取得しただけだと、データが@<code>{message}というfieldに全て入ってしまい扱いにくいものとなってしまいます。
なので@<code>{json}プラグインを使用してjsonを分割します。

//list[filter_notuse_json][jsonの出力例（filterを使用しなかった場合）]{
//}

//list[filter_use_json][jsonの出力例（filterを使用した場合）]{
//}

さらに、データを加工します。

//list[filter_json_logstash][filterプラグインの実装例]{
  jsonを加工するためのプラグインを記載
//}


=== データの送付部（output）
処理が終わったデータをどこに送付するか指定するプラグインです。データの送付以外にも、別フォーマットへの加工が可能です。
例えばjsonで取得したデータをcsvに変換して保存、といったことができます。
また、複数の宛先にデータを送付したい場合、プラグインを複数記述することで実現可能です。

//list[output_json_logstash][outputプラグインの実装例]{
  jsonをcsv取得するためのプラグインを記載
//}

ちなみに、@<code>{stdout}プラグインを使用すると、加工後のデータを標準出力させることが可能です。
serviceコマンドでプロセス起動を行うと、@<code>{logstash.stdout}という名称で標準出力内容が保存されます。
動作確認時に使用すると便利です。

//list[logstash_stdout][stdoutプラグインの例]{
//}

//cmd{
標準出力後の動作を記述
//}


=== その他特徴など
==== 複数のlogstash.confは両立できるが、注意が必要
Logstashは取得するデータごとに@<code>{logstash.conf}を作成し、@<code>{/etc/logstash/conf.d}配下に置いて動作させることができます。

//list[etc_logstash_conf][複数のコンフィグを作成し配置した例]{
  ディレクトリの中を入れる
//}

ただし、Logstashは動作時にコンフィグを全て結合し動作するという特性があります。
なので、各コンフィグ内では読み取ったデータにtag付けを行う処理を入れておかないと意図した処理ができない場合があります。

//list[example_some_logstash_conf][複数のコンフィグを動作させる場合：tag付けを行わなかった場合]{


//}

このコンフィグを動作させると、データは次のように出力されます。

//cmd{
lsした後に起動＆データ重複されていることを確認
//}

なので、Logstashでも複数のデータを同じプロセスで取得する場合、tagをつけてデータを別々に管理する必要があります。
ちなみに、コンフィグを1つにするか複数にするか、どちらがいいのかはElastic社の人も悩みどころらしいです。
個人的には複数に分けて中身を短くする方が管理しやすそうに思いますが、ちょっとめんどいですね。長くてもめんどいのは一緒ですが。

==== add_fieldとremove_field
@<code>{add_field}とは、多くのfilterプラグインについているオプション機能です。
取得したデータに1つfieldを追加できます（複数fieldの追加も可能です）。
@<code>{remove_field}は逆で、指定したfieldを削除することができます。
そのfieldに入っているデータは全部抹消されます。


fieldの数を意図的に操作する、というのは他のElastic製品@<fn>{Elastic_Stack}（特にElasticsearch）との連携を
意識しているものと思われます。Elasticsearchでデータを検索するときや、Kibanaでグラフを描画するときは
field単位でデータを引っ張ってくる感じになるので、意図的にfield数を操作できるようにしているような感じがします。

#@# この辺ちょっと忘れたからあとで調べ直して修正すりーりえ

//footnote[Elastic_Stack][Elastic社製のOSS群はElastic Stackといいます。でもElasticsearch,Logstash,KibanaでELKって言ってる人の方が多いですね。そっちの方が短いし。]

=== Logstashでお便利なプラグイン

Logstashを運用する上でよく出てくるプラグインを紹介します。

==== metrics（filter）
取得したデータの数値型を変換できます。Logstashのバージョンが上がるほど、変換できる型が充実しています。
公式ドキュメントをみると、その充実っぷりをご堪能いただけます。

==== grok（filter）
正規表現を使って、データを細かく分割できるプラグインです。例えばログに含まれるタイムスタンプを
日付データとして使いたいときなどに役立ちます。
○章以降で@micci184さんが使い方を紹介していますので、詳しくはそちらを参照ください。

==== mutate（filter）
指定した条件に一致するデータを変更できます。
ただし、ある程度fieldを分割していないと狙った部分だけ変換、という処理がしにくいので
前に紹介した@<code>{grok}filterをいかに活用し、データを分割できるかが勝負の分かれ目になります。
