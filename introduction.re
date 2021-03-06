= はじめに

りまりま団のもふもふ（Twitter:@froakie0021）です。
Nintendo Switchが欲しいのに、どこも在庫切れで泣いてます。
…というのはさておき、この本ではデータ収集ツールを題材としています。
データ収集ツールはfluentdやLogstashが有名ですが、実際のところ「何が違うのか説明できる？」と言われると
なかなか難しいものがあります。ミドルウェアの選定は難しいですね。

というわけで、この本の前半では有名なデータ収集ツールであるfluentdとLogstashの機能や性能を比較してみました。
後半戦はみっちー（Github:@micci184）さんがLogstashのGrok filter
（Logstashの設定項目の1つ）について熱く語ってます。
ここで、micchi184さんから熱いメッセージが届いておりますので掲載します。

== micchi184さんの熱い意気込み
ゆるふわエンジニアの@micci184ですー

世の中には様々なデータがあり、それがどう活用されているかを知るために、いろんな人たちがログの可視化を行なっています。
しかし、ログの可視化を本格的に実施するためには、何を目的とするかを定義する必要があります。
何かしらの意思決定をするための材料として、ログを活用したい場合がほとんどだからです。

そのためには、ログの整理を実施し、抽出・変換・ログの収集（取り込み）・どのようなダッシュボードを作成するかも考える必要があります。
さらに、非機能面も加味してデザインする必要もでてきます。

ここまで本格的に取り組むとなると。大変な労力と途中で挫折してしまう可能性があります。
なので、ぼくの担当する章では大いなる一歩目のログの抽出・変換・取り込みの楽しさに触れられればと思ってます！

第7章からは、ログの取り込みまでを手助けしてくれるElastic社製のLogstashの使い方や、心が折れやすいGrok Filterについて書いています。
Grokを制することで楽しいロギング生活を送ることが可能になるのです！（って、ぼくは思ってますw）
この本を通じてGrokの苦手意識をなくして頂ければ幸いです。

ゆるふわな感じですが、お付き合い頂ければと思いますーヽ(*ﾟдﾟ)ノ

== 注意事項とか

この本に出てくる各ソフトウェアのバージョンは下記の通りです。

=== 第1章から第6章（CentOS6）
 * fluentd
 ** Version 0.12(td-agent v2.3.5)
 * Logstash
 ** Version 6.0(Beta)

=== 第7章以降（Amazon Linux）
 * Amazon Linux
 ** Amazon Linux AMI 2017.03.1 (HVM)
 ** InstanceType: t2.small
 * Logstash
 ** Version 6.0(Beta)

本の内容は公式ドキュメントなどを元に動作確認を行ってはいますが、
この本の情報を用いた開発・制作・運用に対して発生した全ての結果に対して責任は負うことはできません。
必ずご自身の環境でよく検証してから導入をお願いします。

「こんなのもあるよー」とか「ここは別の知見があるよ」ありましたら、Twitterとかで教えてもらえると嬉しいです。
あと、どーやったらNintendo Switchが買えるのかもついでに教えてもらえるともっと嬉しいです。
全然抽選当たんないよ。マリオオデッセイもめっちゃ面白そうじゃない？

あ、あと途中に出てくるペンギンの可愛さは半端ないのでみなさんも癒されて下さい！ペンギンかわいいね。
