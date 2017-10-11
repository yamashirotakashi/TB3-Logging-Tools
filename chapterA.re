= データ収集ツールとは

データ収集ツールとは、ファイルやアプリケーションなどからデータを集め、別のサービスに転送する
ソフトウェアのことです。主に下記に示すような用途として使われています。

 * サービス監視用途
 ** アプリケーションやサーバーのログの収集
 ** サーバーの負荷情報の収集
 * データ収集用途
 ** Webから収集したデータをストレージへ転送
 ** アプリケーションやストレージから特定のデータを収集
 * サービス間のデータ転送

データ収集ツールは単体でも使用できます。ただし監視用途の場合、データ可視化ツールで集めたデータを可視化ツールで
グラフに整形し分析する、といった使われ方をされることが多いです。
代表的なサービス例としては、Kibana,Grafanaなどがあります。
この本では詳しく扱いませんが、Kibanaによるデータの可視化を行った画面を一例として載せておきます。

//image[example_kibana][データ可視化ツール（Kibana）の例]{
  KibanaのDashboardを挿入
//}

== 代表的なデータ収集ツール
データ収集ツールは有償・OSS問わず多く提供されていますが、この本で主に取り扱う2種類のソフトウェアを簡単に紹介します。

=== fluentd
Tresure Data社製のOSSです（URL:@<href>{https://www.fluentd.org}）。
アイコンがトリのやつです。日本発のせいか、日本語の情報が多いのが特徴です。

Microsoft、Amazon Web Services、Googleなどが利用しており
世界的にも導入事例が多いミドルウェアです。

fluentdはRuby言語で作成されています。各機能はプラグインで提供されています。
特徴は軽量に動作することと、データの安全性が保たれることが挙げられます。
特にデータの安全性担保はかなりの売りとなっており、一度読み取ったデータをディスクに書き込むことができる機能が初期から実装されています。
また、CloudWatchを始めとしたAWSサービスと連携するプラグインが豊富に提供されています。

=== Logstash
Elastic社製のOSSです（URL:@<href>{https://www.elastic.co/jp/products/logstash}）。
アイコンは昔と今でかなり変化があります。興味のある方はぜひ画像検索してみると良いでしょう@<fn>{Elastic_icon}。

//footnote[Elastic_icon][夜にみるとめっちゃ不安になるからやめといたほうがいいです。]


Elastic社のプロダクトは分散処理型の検索エンジンElasticsearchが有名ですが、データ収集ツールも提供しています。
こちらもプラグインはJRubyで作成されているので、Logstashの動作にはJavaが必要です。
そのためか、少し起動は重たい感じがします。

取得したデータのバッファは今まではオンメモリのみでしたが、アップデートでディスクにデータを書き込む機能が追加されました。
この点ではfluentdとほぼ変わらないと言えます。

日本では認知度が低めのソフトウェアですが、Google Trendsで検索数を比較する限り
fluentdよりもLogstashの方が世界的に使われていると言えそうです。@<img>{google_trends}の
棒グラフ左は横線グラフ上と対応しています。白黒だと少しわかりづらいかもしれませんので、実際にGoogle Trendsで
検索してみると面白いと思います。

あと、どちらの関連キーワードにもElasticsearchが挙がっているところが面白いですね。

== 結局どっちも一緒なんじゃないの問題
fluentdとLogstashは機能が似ているソフトウェアです。っていうか側から見るとほとんど同じです。
名前以外の違いがあるのか、これから比較してみたいと思います。

//image[google_trends][Logstashとfluentdの検索数比較]{
//}
