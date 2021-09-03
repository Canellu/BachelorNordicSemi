const char web[] PROGMEM = R"====(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>OASYS CLIENT</title>


    <!-- STYYYYLIIIING  -->
    <style>
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            
        }

        *:focus {
            outline: none;
        }

        body *{
            /* border: 2px solid coral; */
            font-family: Arial;
            
        }

        .container {
            width: 100vw;
            height: 800px;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            align-items: center;
            background-color: #BFBFBF;
            padding: 20px;
          
        }

        .titleBoks {
            margin-top: 20px;
            height: 8%;
            width: 90%;
            color: rgb(240, 240, 240);
            background-color: rgb(95, 156, 255);
            border-radius: 30px;
            display: flex;
            justify-content: center;
            align-items: center;
        }


        .title {
            font-size: 
            28px;
            letter-spacing: 1px;
            font-weight: bold;
        }

        .terminal {
            width: 90%;
            height: 40%;
        }

        .terminalTitle, .filesTitle {
            text-align: center;
            height: 10%;
            font-size: 26px;
            letter-spacing: 2px;
            font-weight: bold;
        }

        .terminalWindow {
            
            padding-top: 8px;
            display: flex;
            justify-content: center;
            align-items: center;
            background-color: rgb(71, 71, 71);
            width: 100%;
            height: 70%;
        }

        .terminal textArea {
            padding: 10px;
            background-color: rgb(139, 139, 139);
            color: white;
            resize: none;
            width: 90%;
            height: 100%;
        }

        .terminalInput {
            display: flex;
            justify-content: space-between;
            align-items: center;
            height: 20%;
            width: 100%;
            background-color:rgb(71, 71, 71);
            padding: 10px;
         
        }

        .textInput {
            /* border-radius: 20px; */
            height: 100%;
            padding-left: 10px;
            resize: none;
            margin-left: 7px;
            width: 100%;
            background-color: #8B8B8B;
            color: white;
            border: none;
        }

        .terminalInput button {
            font-weight: bold;
            color: white;
            width: 80px;
            height: 90%;
            border-radius: 7px;
            border-radius: 7px;
            background: linear-gradient(145deg, #5d5d5d, #4e4e4e);
            margin: 0 10px;
            border: none;
        }


        .terminalInput button:active {
            background-color: coral;
            border-radius: 7px;
            background: #474747;
            box-shadow: inset 5px 5px 10px #3c3c3c,
            inset -5px -5px 10px #525252;
        }

        .files {
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            height: 40%;
            width: 90%;
           
        }

        .fileWindow {
            background-color: rgb(71, 71, 71);
            width: 100%;
            height: 70%;
            padding: 10px;
        }

        .downloadBtn, .showFilesBtn {
            width: 80%;
            height: 12%;
            margin-top: 10px;
            border-radius: 10px;
            border: none;
            background-color: seagreen;
            font-size: 20px;
            font-weight: bold;
            color: rgb(233, 233, 233);
        }

        .downloadBtn:active, .showFilesBtn:active {
            background-color: rgb(31, 90, 57);

        }

        .textAreaSD {
            width: 100%;
            height: 100%;
            background-color: #8B8B8B;
            color: white;
            padding: 10px;
        }

   
      
    </style>




</head>

    <!-- WEB SIDE CONTENT -->
<body>
    <div class="container">
        <div class="titleBoks">
            <div class="title">OASYS ESP CLIENT</div>   
        </div>
       
        <div class="terminal">
            <div class="terminalTitle">Terminal</div>
            <div class="terminalWindow">
                <textarea></textarea>
            </div>
            <div class="terminalInput">
                <input type="text" class="textInput"></textarea>
                <button class="terminalBtn">SEND</button>
            </div>
        </div>
        
        <div class="files">
            <div class="filesTitle">Files in SD</div>
            <div class="fileWindow">
                <textarea class="textAreaSD"></textarea>
            </div>
            <button class="showFilesBtn">Show files</button>
            <button class="downloadBtn">Download from SD</button>
        </div>
    </div>
   

    <!-- Filsaver.js -->
    <script>
        
        var _global = typeof window === 'object' && window.window === window
        ? window : typeof self === 'object' && self.self === self
        ? self : typeof global === 'object' && global.global === global
        ? global
        : this

        function bom (blob, opts) {
        if (typeof opts === 'undefined') opts = { autoBom: false }
        else if (typeof opts !== 'object') {
            console.warn('Deprecated: Expected third argument to be a object')
            opts = { autoBom: !opts }
        }

        // prepend BOM for UTF-8 XML and text/* types (including HTML)
        // note: your browser will automatically convert UTF-16 U+FEFF to EF BB BF
        if (opts.autoBom && /^\s*(?:text\/\S*|application\/xml|\S*\/\S*\+xml)\s*;.*charset\s*=\s*utf-8/i.test(blob.type)) {
            return new Blob([String.fromCharCode(0xFEFF), blob], { type: blob.type })
        }
        return blob
        }

        function download (url, name, opts) {
        var xhr = new XMLHttpRequest()
        xhr.open('GET', url)
        xhr.responseType = 'blob'
        xhr.onload = function () {
            saveAs(xhr.response, name, opts)
        }
        xhr.onerror = function () {
            console.error('could not download file')
        }
        xhr.send()
        }

        function corsEnabled (url) {
        var xhr = new XMLHttpRequest()
        // use sync to avoid popup blocker
        xhr.open('HEAD', url, false)
        try {
            xhr.send()
        } catch (e) {}
        return xhr.status >= 200 && xhr.status <= 299
        }

        // `a.click()` doesn't work for all browsers (#465)
        function click (node) {
        try {
            node.dispatchEvent(new MouseEvent('click'))
        } catch (e) {
            var evt = document.createEvent('MouseEvents')
            evt.initMouseEvent('click', true, true, window, 0, 0, 0, 80,
                                20, false, false, false, false, 0, null)
            node.dispatchEvent(evt)
        }
        }

        // Detect WebView inside a native macOS app by ruling out all browsers
        // We just need to check for 'Safari' because all other browsers (besides Firefox) include that too
        // https://www.whatismybrowser.com/guides/the-latest-user-agent/macos
        var isMacOSWebView = _global.navigator && /Macintosh/.test(navigator.userAgent) && /AppleWebKit/.test(navigator.userAgent) && !/Safari/.test(navigator.userAgent)

        var saveAs = _global.saveAs || (
        // probably in some web worker
        (typeof window !== 'object' || window !== _global)
            ? function saveAs () { /* noop */ }

        // Use download attribute first if possible (#193 Lumia mobile) unless this is a macOS WebView
        : ('download' in HTMLAnchorElement.prototype && !isMacOSWebView)
        ? function saveAs (blob, name, opts) {
            var URL = _global.URL || _global.webkitURL
            var a = document.createElement('a')
            name = name || blob.name || 'download'

            a.download = name
            a.rel = 'noopener' // tabnabbing

            // TODO: detect chrome extensions & packaged apps
            // a.target = '_blank'

            if (typeof blob === 'string') {
            // Support regular links
            a.href = blob
            if (a.origin !== location.origin) {
                corsEnabled(a.href)
                ? download(blob, name, opts)
                : click(a, a.target = '_blank')
            } else {
                click(a)
            }
            } else {
            // Support blobs
            a.href = URL.createObjectURL(blob)
            setTimeout(function () { URL.revokeObjectURL(a.href) }, 4E4) // 40s
            setTimeout(function () { click(a) }, 0)
            }
        }

        // Use msSaveOrOpenBlob as a second approach
        : 'msSaveOrOpenBlob' in navigator
        ? function saveAs (blob, name, opts) {
            name = name || blob.name || 'download'

            if (typeof blob === 'string') {
            if (corsEnabled(blob)) {
                download(blob, name, opts)
            } else {
                var a = document.createElement('a')
                a.href = blob
                a.target = '_blank'
                setTimeout(function () { click(a) })
            }
            } else {
            navigator.msSaveOrOpenBlob(bom(blob, opts), name)
            }
        }

        // Fallback to using FileReader and a popup
        : function saveAs (blob, name, opts, popup) {
            // Open a popup immediately do go around popup blocker
            // Mostly only available on user interaction and the fileReader is async so...
            popup = popup || open('', '_blank')
            if (popup) {
            popup.document.title =
            popup.document.body.innerText = 'downloading...'
            }

            if (typeof blob === 'string') return download(blob, name, opts)

            var force = blob.type === 'application/octet-stream'
            var isSafari = /constructor/i.test(_global.HTMLElement) || _global.safari
            var isChromeIOS = /CriOS\/[\d]+/.test(navigator.userAgent)

            if ((isChromeIOS || (force && isSafari) || isMacOSWebView) && typeof FileReader !== 'undefined') {
            // Safari doesn't allow downloading of blob URLs
            var reader = new FileReader()
            reader.onloadend = function () {
                var url = reader.result
                url = isChromeIOS ? url : url.replace(/^data:[^;]*;/, 'data:attachment/file;')
                if (popup) popup.location.href = url
                else location = url
                popup = null // reverse-tabnabbing #460
            }
            reader.readAsDataURL(blob)
            } else {
            var URL = _global.URL || _global.webkitURL
            var url = URL.createObjectURL(blob)
            if (popup) popup.location = url
            else location.href = url
            popup = null // reverse-tabnabbing #460
            setTimeout(function () { URL.revokeObjectURL(url) }, 4E4) // 40s
            }
        }
        )

        _global.saveAs = saveAs.saveAs = saveAs

        if (typeof module !== 'undefined') {
        module.exports = saveAs;
        }
    </script>


    <script>
        init();
        
        // var showFilesBtn = document.querySelector(".showFilesBtn");
        // showFilesBtn.addEventListener("click", () => {
           
        // })

        var Socket;
        function init() {
          Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
          Socket.onmessage = function(event) {
            console.log(event.data);
            document.querySelector(".textAreaSD").value += event.data;
          }
        }

        var terminalBtn = document.querySelector(".terminalBtn");
        var textInput = document.querySelector(".textInput");
        terminalBtn.addEventListener("click", () => {
            Socket.send(textInput.value);
            textInput.value = "";
        });

       
    </script>
</body>
</html>

)====";
