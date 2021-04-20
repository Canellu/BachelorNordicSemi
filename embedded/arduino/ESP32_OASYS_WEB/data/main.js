let gateway = `ws://${window.location.hostname}/ws`;
let websocket;

// ----------------------------------------------------------------------------
// Initialization
// ----------------------------------------------------------------------------

window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
}

// ----------------------------------------------------------------------------
// WebSocket handling
// ----------------------------------------------------------------------------
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen  = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage; 
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
}

function onMessage(event) {
    console.log(`Received a notification from ${event.origin}`);
    console.log(event.data);
}




// ----------------------------------------------------------------------------
// TEST CODE
// ----------------------------------------------------------------------------
let counter = 0;
let messages = ["Yo", "Test", "Some", "Messages", "Lorem ipsum dolor sit amet consectetur adipisicing elit. Reprehenderit est sed beatae sit expedita? Optio minima placeat nobis voluptas enim." , "Lorem ipsum dolor sit amet consectetur, adipisicing elit. Consequatur doloribus illo, repellendus assumenda magnam distinctio excepturi quaerat perferendis error eaque ad saepe rem quidem facere non. Nesciunt, enim ullam porro natus ipsum nisi rem, esse omnis a corporis totam explicabo dolores inventore, vel facere autem fuga alias numquam quibusdam veritatis voluptates. Doloremque quam eligendi distinctio mollitia necessitatibus iusto molestias dolores voluptatibus nisi architecto ipsam et saepe quo sit molestiae amet veniam ullam velit atque iure aliquid temporibus, iste obcaecati optio. Voluptatibus magnam ratione iusto dolorem quisquam? Saepe, similique quibusdam, et modi hic explicabo nostrum, ipsum blanditiis dolorem suscipit officia beatae!","Lorem ipsum dolor sit amet consectetur adipisicing elit. A quaerat vitae nisi soluta velit pariatur quae, quam culpa ea quis quas libero vel repellendus obcaecati amet, corrupti reprehenderit beatae tempore porro! Cum cupiditate, ab, magni tempore ratione recusandae voluptatibus, nobis ducimus id sed impedit animi nulla enim iure. Quas sit culpa dicta, enim aliquid qui aliquam hic inventore pariatur quasi facilis necessitatibus fuga voluptatibus perferendis! Obcaecati error magni velit cum repudiandae dolore earum exercitationem nobis laboriosam facilis, optio et molestias deleniti dolor quas eius distinctio nesciunt autem minima laborum? Omnis quasi obcaecati suscipit at autem unde facere quibusdam illum? Nobis, ratione cupiditate. Eveniet repudiandae hic nulla omnis facere non voluptatem, perspiciatis voluptas reiciendis magni nesciunt nihil! Asperiores aspernatur quis adipisci nobis nostrum! Harum voluptate enim itaque cupiditate nam eos, earum expedita, incidunt natus veritatis aspernatur, distinctio possimus ratione repellendus nobis. Suscipit, rerum. Eius nobis culpa quaerat tempora quod quam, repellendus odit magni expedita veniam laudantium necessitatibus similique ea tenetur eveniet cumque, est laboriosam enim nesciunt, libero quasi ut? Totam officiis veniam, minus, incidunt ipsum praesentium commodi quae quaerat asperiores blanditiis iusto quibusdam eveniet repudiandae tempore distinctio adipisci excepturi unde, corporis molestiae odio! Corrupti odio ducimus vitae assumenda accusamus deleniti pariatur molestiae eius consequatur asperiores id veniam beatae qui perspiciatis dolor velit nam, amet fuga porro consequuntur. Et tempore inventore nobis impedit natus. Saepe aliquam minus, velit, asperiores veniam dolor magni aperiam quidem dolore itaque laudantium quaerat quis quas enim exercitationem esse ratione eius labore dolorum quam facere nostrum. Temporibus eius et aut hic, commodi est quae unde dolore dolorem rem in voluptates veniam quaerat perferendis magni minima quo consequuntur, ipsam reprehenderit! Ullam unde eum sint optio, vero est architecto reiciendis numquam totam. Atque sequi magni debitis nobis at id quos labore, eum, veniam excepturi, non quaerat asperiores? Iure, reiciendis fugit accusantium, eum quia officiis est culpa molestiae ducimus distinctio architecto aliquam quidem ea aspernatur officia, nesciunt sit velit error magni? Incidunt quo laborum quae eveniet, blanditiis voluptas nobis amet ea earum ipsa accusantium asperiores nemo consequuntur optio possimus aut libero praesentium sequi delectus veritatis. Totam dicta, exercitationem tempora ipsa aspernatur, aut labore eum id earum odio fuga eius recusandae quas, tenetur eveniet hic? Dolore magni corrupti repellat, fugiat error animi sunt. Vel quas soluta, reprehenderit quam iste asperiores facilis quo porro distinctio magnam minus ea? Harum aut molestiae, molestias magnam dolore omnis ex libero fugiat, vitae voluptates minima cum delectus totam. Molestias perspiciatis, quos ipsam nostrum enim voluptatem dicta quasi iusto praesentium iste. Odio voluptates accusamus exercitationem pariatur vel hic earum. Nihil tenetur deleniti dolores? Animi, dolorem unde nesciunt cupiditate dolor culpa asperiores officia illum quo atque doloremque debitis adipisci delectus sequi. Est dolorem distinctio quibusdam voluptate libero necessitatibus sint cupiditate possimus iste dolorum. Rerum beatae sapiente reprehenderit cum culpa maiores animi repudiandae, numquam minima quos cupiditate ipsam eaque deleniti ab excepturi sed non ipsa tenetur, eius illo? Dignissimos facilis provident excepturi consequuntur tempora officiis voluptates, accusamus at necessitatibus dolorum amet nemo distinctio accusantium voluptas fuga, odit animi optio recusandae neque. Facilis voluptates quos ab perferendis quas cupiditate ipsam porro, cumque quae ipsa adipisci possimus id! Iure, commodi officiis. Suscipit incidunt nesciunt obcaecati veritatis, omnis, molestias sapiente beatae est labore ullam eos repudiandae voluptatum. Quis, aliquid nemo libero id pariatur quae nam architecto quam eligendi error optio amet corporis maxime consequatur aperiam itaque natus voluptatum officiis recusandae esse, atque dolores cumque. Provident, expedita, eligendi rem non ut deserunt culpa voluptate perferendis voluptas enim cum ducimus fugiat illo aspernatur voluptates quas quae autem neque laudantium tempore. Tenetur enim doloribus dicta facilis sapiente incidunt laudantium praesentium consectetur itaque aliquid aperiam, a nam alias magnam, laboriosam exercitationem minus quisquam. Alias illum odio non numquam obcaecati, veniam aut. Quam repellendus vero, consequuntur repudiandae adipisci dolores aspernatur debitis rem cum officiis est iure! Animi adipisci accusamus illo sint omnis amet ipsa possimus quas quasi, aperiam placeat facere laudantium expedita aliquid soluta ea tenetur laboriosam veritatis quaerat repellendus quisquam similique esse. Maxime sequi mollitia temporibus impedit beatae est voluptates dolore, deserunt in, commodi id error, fugiat rem tenetur hic eligendi voluptate accusantium cum totam praesentium fugit nam facilis? Alias, magnam? Mollitia saepe minus quos deserunt commodi deleniti quibusdam quaerat magnam assumenda asperiores odit corrupti non soluta, vitae corporis expedita delectus omnis atque similique culpa incidunt. Vel nobis accusamus, ipsam, in hic sunt voluptates sit similique excepturi consectetur enim rem nemo molestiae reprehenderit? Non sint a corporis et quae amet, enim est harum, sequi quam cum quia, provident in vero molestias debitis deserunt aliquid necessitatibus! Iusto ad architecto ipsum totam accusantium repudiandae, quam facilis nisi aperiam? Velit earum modi sapiente impedit dolores rerum fugiat dolore quod rem fuga accusantium tempore dolor, quia, illum vel animi sit quos eligendi. Incidunt provident repellendus voluptas dolor porro corporis, dolores, nam, quae ad doloribus maxime dignissimos aspernatur consectetur magnam odit quis illum in. Voluptatem, quo voluptate. Expedita, eos earum excepturi labore voluptates voluptate velit quia porro exercitationem tenetur sed dolorum. Fugiat quod quae quisquam praesentium omnis iusto animi. Maxime aspernatur deleniti qui, magni laudantium eos necessitatibus et amet officia eum unde quas dolorum culpa expedita consectetur tenetur ab dolorem! Quas qui, tenetur animi vitae, quaerat repellendus, sapiente ipsa laboriosam architecto doloremque cumque? Accusantium odio earum quibusdam inventore ullam deserunt dolor iusto totam porro neque tenetur, eos aliquam? Officia architecto facilis perspiciatis, at ea culpa rem, neque, quam nostrum pariatur amet quibusdam doloribus exercitationem totam. Ea ipsum voluptatum corrupti maiores tempora, repellendus excepturi culpa fuga sequi ipsam, cumque dolor voluptate, voluptatem eum dolore animi. Itaque quo vero, molestias eius praesentium quae! Illum atque impedit molestiae magnam amet rem id voluptatibus sapiente cum dolorem, culpa alias vel vero dicta dolorum, aut nemo eveniet, minus aliquid! Nulla sequi doloribus officia assumenda amet reiciendis, asperiores consectetur eos, a fugit corporis magnam similique quam ut sapiente ipsam blanditiis. Sapiente autem possimus ab iste neque quisquam nemo, harum recusandae accusamus, quidem sit impedit rerum labore quis laboriosam provident reprehenderit dicta corrupti mollitia! Vel ad facilis sequi autem totam? Fuga est accusantium perferendis dolor tenetur repellat in! Culpa?","Lorem ipsum dolor sit amet consectetur adipisicing elit. Maiores architecto rem explicabo consectetur id dolore quia aliquam quos? Ab accusantium temporibus ullam nostrum culpa cumque perferendis alias rerum officiis optio fugiat eaque unde quisquam dolorum, impedit sapiente minus quos. Exercitationem excepturi delectus possimus. Nulla nesciunt atque est aliquam consectetur, velit reiciendis maiores amet quaerat culpa, cupiditate, labore quas optio architecto incidunt. Rerum at nesciunt voluptatem consectetur! Maxime repudiandae suscipit dolor vel consectetur commodi vero recusandae, minus ullam quia nobis voluptatibus aperiam esse ex veniam deleniti! Aliquid quae fugit doloribus enim quia repellat odit, totam, quidem autem, atque illum aliquam. Ex, dolores dignissimos amet vero qui, ipsam nobis sequi suscipit recusandae sunt fugiat quibusdam id blanditiis aliquam provident possimus non! Quidem veritatis laudantium saepe voluptas placeat, assumenda consectetur praesentium, nesciunt laboriosam accusamus blanditiis eaque debitis dignissimos atque sequi a fugiat accusantium iste voluptates vel? Doloribus repudiandae similique, eaque culpa velit, laborum itaque id in maiores optio omnis consequatur atque quos libero praesentium vel cumque voluptas, autem doloremque magni. Minima quisquam, molestiae esse mollitia eius nulla et error sint. Debitis fuga voluptatum tempora perferendis ut quo odio repudiandae expedita labore delectus? Veritatis ad aliquid ex recusandae id, rerum accusantium hic dolor quo, a quidem mollitia cupiditate et illo corporis obcaecati ullam perspiciatis animi quia necessitatibus molestias. Perferendis vitae, qui atque doloribus nesciunt aliquam cupiditate, dignissimos repellat consequuntur maxime ratione obcaecati delectus accusantium minus reiciendis quaerat, nobis blanditiis deserunt voluptas! Dolorem eveniet quaerat error quod est molestias officiis atque laborum dolor similique inventore asperiores quidem debitis ad, minus quas corrupti suscipit quibusdam, quia aperiam. Animi officiis voluptatum architecto, quaerat deleniti quia fuga dicta facilis quos repudiandae repellat accusamus impedit! Veniam tempora eius consequatur velit voluptatum quos quisquam enim explicabo consequuntur, ratione laborum maxime? Doloribus rem impedit animi nam aut harum sint excepturi sequi, tempora numquam aspernatur, libero, blanditiis autem dolorum quos debitis reiciendis molestias! Fugit iure nesciunt quidem officiis eveniet iste pariatur, excepturi facilis aut? Aliquam rem nobis omnis molestias molestiae corporis assumenda eos? Amet voluptatibus rem quaerat. Voluptatibus nulla labore dolores repellendus, voluptatum reprehenderit veniam repellat velit. At modi facilis impedit assumenda commodi doloribus eum delectus quod porro a nostrum unde numquam, ad quidem velit debitis asperiores fugiat quis repellendus sapiente, repudiandae nihil. Quisquam rerum nihil magnam vel aliquam sint! Consequatur vero placeat facilis officia. Molestias voluptatum quidem quas ratione nostrum quos, excepturi culpa quis delectus, praesentium beatae labore, asperiores porro sit quia mollitia voluptates sapiente? Obcaecati, natus accusamus veniam repudiandae quo doloremque officiis maxime blanditiis cupiditate iusto incidunt similique necessitatibus beatae ut voluptate reprehenderit delectus placeat in nisi alias deserunt. Explicabo dolorum nesciunt deserunt quidem nisi quibusdam vel eveniet sequi officiis voluptatum hic labore animi minima, magnam sapiente incidunt rerum, nemo impedit, laboriosam cupiditate alias sed. Dolorem totam, rem numquam commodi ab, praesentium aliquam unde sunt soluta inventore dolore quaerat dolores quam? Deserunt cupiditate iure praesentium minus, odio ab iusto earum quo sapiente expedita. Quidem inventore, cumque neque, architecto iusto deleniti minus consectetur iste voluptates explicabo aliquam? Praesentium, consequuntur vero unde maxime voluptatem sint laborum est eligendi voluptate! Nobis, mollitia. Eligendi excepturi sit velit officiis voluptate ullam aliquam quod vitae nobis molestias? Nihil accusantium dolores tempora, ab dolorum eligendi itaque blanditiis est provident rem ducimus vel sunt iste, omnis tenetur obcaecati. Nisi, vel officia minus, quas necessitatibus sapiente veritatis enim incidunt placeat, commodi adipisci tenetur eius temporibus obcaecati repudiandae quae recusandae dolor mollitia quod! Cum sequi iusto repellat sunt alias rem exercitationem doloribus laborum dolorem, nisi non voluptatem. Sapiente hic magni debitis natus, praesentium blanditiis? Nostrum dolores, soluta aspernatur temporibus impedit sequi ex dignissimos dicta iusto eveniet libero assumenda! Corporis neque, placeat dicta quis amet ea temporibus ipsa possimus porro soluta perspiciatis! Impedit dolorum obcaecati fugiat incidunt rerum provident necessitatibus officiis est facilis. Pariatur temporibus deleniti officia fuga suscipit architecto adipisci eveniet ea, perferendis eius saepe possimus praesentium eligendi exercitationem vero, enim voluptate delectus. Numquam nobis est necessitatibus temporibus a aperiam, obcaecati illo molestias repellat inventore fuga. Suscipit magnam facere nihil ratione velit quisquam similique? Ab perspiciatis enim a laudantium ratione distinctio nemo, beatae maxime ut obcaecati quaerat modi accusamus facilis quo expedita, hic iste itaque nihil molestiae dolor, nulla cum provident rerum. Odio obcaecati quibusdam inventore veritatis sint id culpa voluptate labore autem eos et quae ratione aliquam vitae minima laudantium nisi explicabo suscipit, in soluta. Voluptatum possimus aspernatur qui aliquam, quibusdam perspiciatis nobis doloribus. Tempora recusandae eveniet culpa quo dolorum iure quam soluta doloribus voluptatum, ea beatae. Cumque similique, dignissimos delectus nobis et dolores reiciendis iste omnis corporis cupiditate illo, sit, eos optio asperiores repellat beatae quaerat placeat aspernatur sed odio perspiciatis quisquam numquam? Natus eum optio nulla perspiciatis laborum esse soluta libero blanditiis itaque vel quos dolore tempore saepe modi ut quidem minus necessitatibus accusantium neque distinctio, amet veritatis nobis dolor! Eum, sapiente amet delectus aliquam eligendi corporis veniam tenetur velit aspernatur libero. Aperiam veniam aspernatur ratione esse autem inventore vitae distinctio, nisi eveniet, facere id amet suscipit repellat recusandae labore laudantium eum commodi aliquam vero reiciendis quod est? Nostrum, explicabo minus! Corrupti aperiam iusto sequi illo voluptatem eum qui dignissimos, cupiditate eaque quo aliquid eligendi ea, odio inventore, molestias est ullam eveniet repellat quod nostrum optio pariatur earum? Corrupti, libero optio atque, provident commodi voluptatem nostrum tempore consequuntur eius veniam necessitatibus cumque quasi maiores quo! Doloremque accusamus distinctio consectetur voluptates saepe aut dicta minus fugiat nostrum beatae consequuntur assumenda iste, voluptate atque ad omnis asperiores modi, corporis optio. Ex, debitis quae eveniet asperiores perspiciatis quod. Explicabo nostrum deleniti asperiores quos placeat suscipit sapiente ut debitis temporibus quae quis, optio neque incidunt quod aut dolores perspiciatis adipisci unde doloribus. Quam earum fugiat alias accusamus impedit adipisci dicta sint distinctio mollitia soluta dolore, iusto dolorem quasi quo id repudiandae nobis quas. Quas modi ut nemo consequuntur unde! Reprehenderit amet adipisci quasi perferendis, possimus optio accusamus eius rerum nesciunt quod nihil explicabo obcaecati accusantium ducimus provident facere sequi officiis excepturi! Quaerat aliquam quos, pariatur sequi autem rerum blanditiis odio delectus possimus facilis eum maxime animi laborum omnis. Ducimus aliquid, a pariatur est nulla quo natus possimus odio veniam ad ex dolore ab animi quisquam, fuga beatae repellat explicabo fugit cupiditate. Omnis repudiandae minima dolorem ab odit dolorum nostrum sapiente fugiat explicabo est debitis ducimus corporis, hic sunt quas quasi veritatis tempora rem error aut eos doloribus? Voluptatibus ipsam nam laboriosam non. Cupiditate dolorem sint cum soluta doloremque facere qui sed esse magni? Quae, est quam quis neque molestias eveniet, voluptas dolorem quos pariatur hic, labore reiciendis consequatur tenetur! Blanditiis, reprehenderit modi doloremque molestiae perferendis reiciendis assumenda, quis minus hic, officia unde nostrum! Blanditiis quaerat animi aut molestias iusto amet rem cupiditate alias pariatur soluta suscipit, consequuntur doloremque saepe velit possimus architecto vel voluptate. Iure, ab ullam fuga fugiat incidunt rem distinctio veniam, culpa molestiae autem hic ipsa perferendis quos velit. Ducimus error est cupiditate magni! Enim eos voluptatum autem ex in totam voluptate odit quidem quae aperiam nam, illum quas, nisi non ipsa eveniet provident sint possimus cupiditate magnam? Inventore, neque laborum laboriosam hic explicabo repudiandae id in aperiam illum molestiae doloribus cupiditate, amet saepe quaerat nihil dolor iure iusto quidem. Cum nam, laboriosam officia ab eligendi inventore maxime soluta, rerum vero laudantium cumque expedita accusantium iure eius. Aut, illum? Quia earum repellat iusto placeat, voluptatibus laborum tenetur fugit alias dolorum doloremque. Alias, quaerat nemo! Ipsa, eaque a aliquam facere commodi iusto fugit soluta repudiandae eveniet nesciunt. Quaerat magni, est quasi iste unde necessitatibus autem hic consequatur fugit nostrum nobis ullam modi impedit aperiam, vel nesciunt rem officiis odio? Eveniet accusamus culpa, est sit officia reiciendis! Blanditiis maiores ex perferendis quisquam accusamus officia optio ipsum aliquam natus incidunt ipsam nisi aut veritatis ipsa exercitationem dignissimos possimus aspernatur fuga magni iusto, qui accusantium beatae. Nesciunt doloremque accusamus necessitatibus quia! Labore saepe doloribus magnam sequi non nesciunt blanditiis dignissimos laboriosam molestias, reprehenderit dolorem, voluptatibus optio culpa perferendis iste, beatae asperiores deserunt libero omnis! Dolorum perferendis, dolore, ipsum rem impedit vitae blanditiis odio adipisci dolor fuga ratione repudiandae ab! Reiciendis voluptatibus commodi sit nostrum eligendi nihil dolores temporibus architecto dicta maxime non fugiat repellat at neque odio, laborum perferendis earum quos. Itaque fugit, perferendis ipsum expedita quis assumenda aperiam, alias eius voluptatibus velit error rem temporibus aspernatur. Non ipsum impedit quasi sunt perferendis nesciunt totam. Perspiciatis, ipsam facilis. Enim corrupti delectus, recusandae quis similique pariatur, quos amet id ab eum temporibus, nulla labore provident. Doloremque molestiae minus asperiores illo deleniti dolores, ipsam obcaecati quam veniam voluptatem, aspernatur dolore ratione nisi distinctio hic. Voluptas ipsum reiciendis nulla asperiores mollitia necessitatibus, sapiente error omnis similique pariatur aperiam, consequatur quam quasi quos ea aliquid tenetur dolores deleniti, laboriosam quas unde. Voluptatum quos sequi aperiam optio animi quas, et perferendis laborum corporis sint distinctio, odio dolores totam! Harum, amet in? Inventore mollitia laudantium autem quod veniam placeat. Quas aliquam eaque, similique autem necessitatibus odit ratione excepturi vel delectus assumenda tempora dolorem soluta. Amet sint sit expedita adipisci repudiandae alias ab rerum repellendus, numquam dolores optio ipsa. Ipsam adipisci sed cum blanditiis optio? Similique, cumque commodi error qui sunt quos consequuntur a, dignissimos corrupti odit aperiam, eaque nam voluptas? Ex numquam suscipit eos doloribus. Ex saepe perspiciatis mollitia sint minus laborum velit natus iure laudantium possimus iusto optio eum quaerat aliquid aperiam molestias similique inventore eos dicta, ad voluptas ab! Amet in, ipsam alias esse voluptate temporibus cumque ab quos, saepe ipsa pariatur. Minus iure quidem modi doloremque cumque adipisci, perspiciatis esse in illum reiciendis non dolorem repellat sunt itaque placeat minima incidunt culpa libero explicabo accusantium ducimus. Adipisci, error animi dignissimos, esse eos suscipit iusto autem debitis optio molestias dolorum quos, aspernatur et quisquam iste consectetur aliquam nihil! Nulla quae delectus alias dolores aliquam ea corrupti suscipit! Enim eius tenetur reprehenderit ut autem accusamus minus porro, neque voluptate, alias recusandae! Consequatur tenetur nam reiciendis, earum voluptas numquam ducimus necessitatibus dicta exercitationem nesciunt facere voluptatibus delectus eius voluptates? Repellendus harum voluptatem sint adipisci repellat facilis recusandae illum nihil vitae, consectetur porro nam ratione repudiandae numquam magnam doloremque sunt quidem dolore quod assumenda nemo. Ex voluptatum sint laborum saepe sapiente molestiae exercitationem nostrum ducimus perspiciatis minus quo, officiis architecto eveniet eaque odio hic illo est doloribus amet! Facere nesciunt, laudantium mollitia laborum delectus, officiis placeat pariatur quis ullam numquam harum suscipit quidem exercitationem eligendi laboriosam ipsam porro totam repellat. At, minima temporibus provident aut ullam error consectetur, itaque repudiandae dolores quis consequuntur saepe suscipit, reiciendis animi voluptate amet autem modi et quam debitis quae quos voluptatum. Quasi dicta beatae veniam veritatis a! Sunt exercitationem molestiae dolore nisi optio facere quisquam quo, velit ad quos pariatur cupiditate reprehenderit. Libero quisquam eveniet atque aliquam laudantium minima autem voluptatem? Saepe id inventore ipsa nostrum numquam exercitationem aliquam ducimus. Provident assumenda, sapiente earum, labore quam eligendi excepturi obcaecati minus veritatis vel, dolores eveniet dignissimos sint laudantium aspernatur quibusdam quis unde quasi nihil facere magnam neque doloremque. Perspiciatis quia, culpa excepturi vel deleniti vitae necessitatibus eveniet. Quod est pariatur quia consequatur, cupiditate sunt incidunt minima reprehenderit, quidem esse, neque modi consectetur? Debitis ad doloremque adipisci illo possimus? Id, alias magnam. Commodi optio maiores dicta reprehenderit! Eos repudiandae tempore velit dolore deleniti est, impedit totam delectus maxime, ex praesentium, deserunt fugiat laborum quos fuga in accusamus facilis debitis soluta placeat quasi saepe expedita vitae? Reiciendis error et placeat sequi totam id aperiam. Inventore nesciunt, laudantium eligendi amet nulla possimus adipisci accusamus et assumenda itaque labore voluptatum nostrum iste rerum soluta consequatur deleniti quod quam reiciendis esse. Molestias est impedit harum! Beatae neque ab sed pariatur? Facere quo minima, qui saepe consequatur non eos in tempora. Eligendi dolorem autem fuga explicabo corrupti quisquam, delectus fugit ducimus porro rem ipsam reiciendis sequi quidem iure molestias, itaque nihil id recusandae non vero labore! Omnis, aspernatur sit fuga illo excepturi dolores quisquam quidem recusandae rem ut, maiores hic inventore quae, laboriosam aut ipsa culpa ab! Placeat sed iure iusto? Aspernatur quam, vitae dolorem excepturi amet odit, eum et incidunt temporibus inventore officia, laborum sapiente iusto voluptatum corrupti reiciendis laudantium provident voluptate? Odio repudiandae eveniet repellendus iusto. Officia, possimus voluptas."]


let textarea = document.querySelector(".textarea");
let sendarea = document.querySelector("#sendtarea");
function logToConsole() {
    if(counter < messages.length) {
        sendarea.innerHTML = messages[counter];
        websocket.send(messages[counter]);
    }
    textarea.innerHTML = ++counter;
}

