const char edit_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en">
  <head>
    <title>File manager</title>
    <style type="text/css" media="screen">
      body {
        font-size: 12px;
        font-family: Verdana, Arial, Sans-serif;
      }
      .contextMenu {
        z-index: 300;
        position: absolute;
        left: 5px;
        border: 1px solid #444;
        background-color: #F5F5F5;
        display: none;
        box-shadow: 0 0 10px rgba( 0, 0, 0, .4 );
        font-size: 11px;
        font-weight: bold;
      }
      .contextMenu ul {
        list-style: none;
        top: 0;
        left: 0;
        margin: 0;
        padding: 0;
      }
      .contextMenu li {
        position: relative;
        min-width: 60px;
        cursor: pointer;
      }
      .contextMenu span {
        color: #444;
        display: inline-block;
        padding: 6px;
      }
      .contextMenu li:hover { background: #444; }
      .contextMenu li:hover span { color: #EEE; }
    
      .css-treeview ul, .css-treeview li {
        padding: 0;
        margin: 0;
        list-style: none;
      }
      .css-treeview input {
        position: absolute;
        opacity: 0;
      }
      .css-treeview {
        font-size: 11px;
        -moz-user-select: none;
        -webkit-user-select: none;
        user-select: none;
      }
      .css-treeview span {
        color: #00f;
        cursor: pointer;
      }
      .css-treeview span:hover {
        text-decoration: underline;
      }
      .css-treeview input + label + ul {
        margin: 0 0 0 22px;
      }
      .css-treeview input ~ ul {
        display: none;
      }
      .css-treeview label, .css-treeview label::before {
        cursor: pointer;
      }
      .css-treeview input:disabled + label {
        cursor: default;
        opacity: .6;
      }
      .css-treeview input:checked:not(:disabled) ~ ul {
        display: block;
      }
      .css-treeview label, .css-treeview span, .css-treeview label::before {
        background: url(" data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAACgCAMAAAAy5xwmAAAAb1BMVEVHcEyWm55WW2E6QUdAQEBiaG1jaG7///8vNj3KzM7CxMbm5ufMzc/Y2dqxtLaGio58gYWJjZFRVFZJTE/Lzc+vsbNdXV74+PhvdHlsbW2kp6q7vLzy8vMuNTzf39+ho6UyOUCFhYWUl5nv7+/p6ekQi/GRAAAAAXRSTlMAQObYZgAAASRJREFUSMftk9tywiAQhoUAhVAwJpqDRqfavv8zluNKkurEjrY3+e74JizLn2W1miAKxphOBMeUYq6uH/A3A66YpRBRqJZaMI8iwsV/CdM6A2xj5nIUsK0/iTUPrGNm0MUDIkeG/J54uOiksb9hXh55PhIIjfKgdJTHSEy2/FD06Xmod8NOwrrwFUsQmXZHZhhekHBiByYIoaIJAjP8USWiCA8KhEW8UGT7KPaZExsObHyrUgTkjTz8liPXwzwEB+OPFVxHE8X2ckRByLaVyhVKxP27KN+nAtH7Rvur2Dr621tekMes+WiG86Hr6jKYj+7Q1Kd0PpxI58NsQdN/25zlQDQdKb9AnM6fHSGkPsQaJamIpQzCrywhD1iT2XksLCws/IZv8Zof7t25YlkAAAAASUVORK5CYII=") no-repeat;
      }
      .css-treeview label, .css-treeview span, .css-treeview label::before, .css-treeview span::before {
        display: inline-block;
        height: 16px;
        line-height: 16px;
        vertical-align: middle;
      }
      .css-treeview label {
        background-position: 18px 0; /* folder icon */
      }
      .css-treeview span {
        background-position: 18px -48px; /* generic file icon */
      }
      .css-treeview span.txt {
        background-position: 18px -64px; /* text file icon */
      }
      .css-treeview span.img {
        background-position: 18px -80px; /* image file icon */
      }
      .css-treeview label::before {
        content: "";
        width: 16px;
        margin: 0 22px 0 0;
        vertical-align: middle;
        background-position: 0 -32px; /* expand icon */
      }
      .css-treeview span::before {
        content: "";
        width: 16px;
        margin: 0 22px 0 0;
      }
      .css-treeview input:checked + label::before {
        background-position: 0 -16px; /* collapse icon */
      }
      
      /* webkit adjacent element selector bugfix */
      @media screen and (-webkit-min-device-pixel-ratio:0)
      {
        .css-treeview{
          -webkit-animation: webkit-adjacent-element-selector-bugfix infinite 1s;
        }

        @-webkit-keyframes webkit-adjacent-element-selector-bugfix 
        {
          from  { 
            padding: 0;
          } 
          to  { 
            padding: 0;
          }
        }
      }
      
      #header { 
        position: absolute;
        top: 0;
        right: 0;
        left: 0;
        height:24px;
        padding: 1px 1px 0px 10px;
        background-color: #444;
        color:#EEE;
      }
      #tree { 
        position: absolute;
        top: 25px;
        bottom: 0;
        left: 0;
        width: 20%;
        padding: 8px;
      }
      #editor, #preview { 
        position: absolute;
        top: 25px;
        right: 0;
        bottom: 0;
        left: 20%;
      }
      #preview {
        background-color: #EEE;
        padding:5px;
      }
      
      #status {
        position: absolute;
        top: 3px;
        right: 10px;
        font-size: 15px;
      }
      
      #fsMeter {
        width:100px;
        padding-bottom:2px;
      }
      
      #warning {
        height:100%;
        background-color:orange;
        color:black;
      }
      .tooltip {
        position: absolute;
        z-index: 2; /* must be above editor which is at 1 */
        right:0px;
        top:20px;
        visibility: hidden;
        background-color: white;
        color: black;
        text-align: center;
        border: 1px solid #000;
        padding: 3px;
        font-size: 10px;
      }
      #warning:hover .tooltip {
        visibility: visible;
      }

      #loading {
        position:absolute;
        // note: changing between block and none cancels the opacity anim, so we move it in and out of screen instead by changing the top value
        // maybe see https://www.impressivewebs.com/animate-display-block-none/ for another approach
        display:block; 
        top: -100vh;
        left: 0;
        width: 100vw;
        height: 100vh;
        z-index: 100;
        background-color: rgba(0, 0, 0, 0.5);
        opacity: 0;
        transition: opacity 500ms ease-in-out;
      }
      #loading.shown {
        top: 0;
        opacity: 1;
      }
      
      #loading-msg {
        display: inline-block;
        position: absolute;
        top: 0px;
        left: 50%;
        transform: translateX(-50%);
        color:white;
        font-size: 32px;
      }

      @keyframes spinner-anim {
        0% {
          transform: rotate(0deg);
        }
        100% {
          transform: rotate(360deg);
        }
      }
      /* :not(:required) hides this rule from IE9 and below */
      .spinner-anim:not(:required) {
        display: inline-block;
        position: relative;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
        animation: spinner-anim 1s infinite linear;
        border: 16px solid #eee;
        border-right-color: transparent;
        border-radius: 32px;
        box-sizing: border-box;
        overflow: hidden;
        text-indent: -9999px;
        width: 64px;
        height: 64px;
      }
    </style>
    <script>
      var tree;   // needed by editor to refresh file size on save
      var fsInfo; // needed to change refreshPath behaviour on SPIFFS
      /////////////////////////
      // UTILS

      function setLoading(show, message) {
        if (message) console.log(message);        
        document.getElementById("loading-msg").innerHTML = message?message:"";
        if (show) document.getElementById("loading").classList.add("shown");
        else      document.getElementById("loading").classList.remove("shown");
        document.body.style.cursor = show?"wait":"default";
      }

      function readableSize(bytes) {
        if (bytes < 1024) return bytes + " B";          
        var units = [' KiB', ' MiB', ' GiB', ' TiB', 'PiB'];
        var i = -1;
        do {
          bytes = bytes / 1024;
          i++;
        } while (bytes > 1024);
        return bytes.toFixed(2) + units[i];
      }

      function refreshStatus(){
        document.getElementById("status").innerHTML = "(refreshing...)";
        var xmlHttp = new XMLHttpRequest();
        xmlHttp.onload = function() {
          if (xmlHttp.status != 200) showHttpError(xmlHttp);
          else {            
            fsInfo = JSON.parse(xmlHttp.responseText);
            var status = fsInfo.type + " - ";
            if (fsInfo.isOk) {
              var percent = (1+Math.round(99*fsInfo.usedBytes/fsInfo.totalBytes)); // fake to see the "used" bar in any case
              var text = readableSize(fsInfo.totalBytes - fsInfo.usedBytes) + " free of " + readableSize(fsInfo.totalBytes);
              status += "<meter id='fsMeter' min='0' optimum='0'low='90' high='95' max='100' value='" + percent + "' title='" + text + "'>" + text + "</meter>"
              if (fsInfo.unsupportedFiles) {
                status += " <span id='warning'>WARNING<span class='tooltip'>"
                  + "Filesystem&nbsp;contains&nbsp;unsupported&nbsp;filenames:<br/>"
                  + fsInfo.unsupportedFiles
                  + "</span></span>";
              }
            } 
            else {
              status += "<span style='background-color:red;color:white;font-weight:bold'>INIT ERROR !</span>";
            }
            document.getElementById("status").innerHTML = status;
            if (fsInfo.type != "SPIFFS") {
              document.getElementById("mkdir").style.display = "inline";
            }
          }
        };
        xmlHttp.open("GET", "/status", true);
        xmlHttp.send(null);
      }
      
      function showHttpError(request) {
        alert("ERROR: [" + request.status+"] " + request.responseText);
      }
           
      function canLoadNewContents() {
        // The fact the save button is enabled indicates the editor has unsaved changes
        if (document.getElementById("saveBtn").disabled) return true;
        if (confirm("Changes to your document will be lost if you continue")) {
          enableSaveDiscardBtns(false);
          return true;
        }
        return false;
      }

      function enableSaveDiscardBtns(enabled) {
        document.getElementById("saveBtn").disabled = !enabled;
        document.getElementById("discardBtn").disabled = !enabled;
      }


      /*
       * Returns the parent folder of a given path
       * ""      > ""
       * "/"     > ""
       * "a"     > ""
       * "/a"    > ""
       * "a/"    > ""
       * "/a/"   > ""
       * "a/b"   > "/a"
       * "/a/b"  > "/a"
       * "a/b/"  > "/a" 
       * "/a/b/" > "/a" 
       */
      function getParentFolder(path) {
        if (!path.startsWith("/")) path = "/" + path;
        if (path.endsWith("/")) path = path.slice(0, -1);
        return path.substring(0, path.lastIndexOf("/"));        
      }


      /////////////////////////
      // HEADER with uploader, buttons ans status

      function createHeader(element, tree, editor){       
        var header = document.getElementById(element);
        var xmlHttp;
        var fileSelector = document.createElement("input");
        fileSelector.type = "file";
        fileSelector.multiple = false;
        fileSelector.name = "data";
        header.appendChild(fileSelector);
        
        var pathInput = document.createElement("input");
        pathInput.id = "pathInput";
        pathInput.type = "text";
        pathInput.name = "path";
        pathInput.defaultValue = "/";
        header.appendChild(pathInput);

        var upload = document.createElement("button");
        upload.innerHTML = "Upload";
        header.appendChild(upload);

        var mkdir = document.createElement("button");
        mkdir.id = "mkdir";
        mkdir.style.display = "none";
        mkdir.innerHTML = "MkDir";
        header.appendChild(mkdir);

        var mkfile = document.createElement("button");
        mkfile.innerHTML = "MkFile";
        header.appendChild(mkfile);

        var editorButtons = document.createElement("span");
        editorButtons.id = "editorButtons";
        editorButtons.style.display = "none";
        header.appendChild(editorButtons);

        var save = document.createElement("button");
        save.id = "saveBtn";
        save.innerHTML = "Save";
        save.style.marginLeft = "30px";
        save.disabled = true;
        editorButtons.appendChild(save);
  
        var discard = document.createElement("button");
        discard.id = "discardBtn";
        discard.innerHTML = "Discard";
        discard.disabled = true;
        editorButtons.appendChild(discard);
  
        var help = document.createElement("button");
        help.id = "helpBtn";
        help.innerHTML = "?";
        editorButtons.appendChild(help);
  
        var status = document.createElement("span");
        status.id = "status";
        status.innerHTML = "(loading)";
        header.appendChild(status);
  
        fileSelector.onchange = function(e){ // A file has been selected
          if (fileSelector.files.length === 0) return;          
          var filename = fileSelector.files[0].name;
          if (mustFormat83()) {
            filename = to83(filename);
          }
          if (!pathInput.value.startsWith("/")) pathInput.value = "/" + pathInput.value;
          pathInput.value = getParentFolder(pathInput.value) + "/" + filename;
        }

        upload.onclick = function(e) {
          if (fileSelector.files.length === 0) return;   
          tree.httpUpload(fileSelector.files[0], pathInput.value);
        }

        function mustFormat83() {
          return false; // TODO: if needed, test fsInfo.type (and fatType ?)
        }

        function to83(filename) {
          var ext = /(?:\.([^.]+))?$/.exec(filename)[1];
          var name = /(.*)\.[^.]+$/.exec(filename)[1];
          if (name !== undefined){
            if (name.length > 8) name = name.substring(0, 8);
            filename = name;
          }
          if (ext !== undefined){
            if (ext === "html") ext = "htm";
            else if (ext === "jpeg") ext = "jpg";
            filename = filename + "." + ext;
          }
          return filename;
        }

        mkfile.onclick = function(e){
          var path = pathInput.value.trim();
          if (path === "") alert("A filename must be given");
          else if (path.endsWith("/")) alert("Filenames must not end with a '/' character");
          else tree.httpCreate(path);
        };
        
        mkdir.onclick = function(e){
          var path = pathInput.value.trim();
          if (path === "") alert("A folder name must be given");
          else {
            if (!path.endsWith("/")) path = path + "/";
            tree.httpCreate(path);
          }
        };
        
        save.onclick = function(e){
          editor.save();
        }
        
        discard.onclick = function(e){
          editor.discard();
        }
        
        help.onclick = function(e){
          editor.showShortcuts();
        }
        
      }
      
      
      /////////////////////////
      // FILE TREE

      function createTree(element, editor){
        var preview = document.getElementById("preview");
        var treeRoot = document.createElement("div");
        treeRoot.className = "css-treeview";
        treeRoot.id = "/";
        document.getElementById(element).appendChild(treeRoot);
  
        function loadDownload(path){
          document.getElementById("download-frame").src = path+"?download=true";
        }
        
        function loadImgPreview(path){
          document.getElementById("pathInput").value = path;
          document.getElementById("editor").style.display = "none";
          document.getElementById("editorButtons").style.display = "none";
          preview.style.display = "block";
          preview.innerHTML = "<img src='"+path+"' style='max-width:100%; max-height:100%; margin:auto; display:block;' />";
        }
        
        // Fall back code used when ace.js is not available (neither online or local)
        function loadTxtPreview(path){
          document.getElementById("pathInput").value = path;
          document.getElementById("editor").style.display = "none";
          document.getElementById("editorButtons").style.display = "none";
          preview.style.display = "block";
          preview.innerHTML = "<span style='color:red;'>Ace editor could not be loaded from the internet nor from /edit/ace.js . Defaulting to text viewer...</span><pre id='txtContents' style='overflow: auto;'></pre>";
          var xhr = new XMLHttpRequest();
          xhr.onload = function () {
            document.getElementById('txtContents').textContent = this.responseText;
          };
          xhr.open('GET', path);
          xhr.send();
        }
        
        this.clearMainPanel = function() {
          document.getElementById("editor").style.display = "none";
          document.getElementById("editorButtons").style.display = "none";
          preview.style.display = "block";
          preview.innerHTML = "<div style='text-align:center'>(file not found or format not supported)</div>";
        }
    
        function isTextFile(path){
          var ext = /(?:\.([^.]+))?$/.exec(path)[1];
          if (ext !== undefined){
            switch(ext.toLowerCase()){
              case "txt":
              case "htm":
              case "html":
              case "js":
              case "json":
              case "c":
              case "h":
              case "cpp":
              case "css":
              case "xml":
                return true;
            }
          }
          return false;
        }
  
        function isImageFile(path){
          var ext = /(?:\.([^.]+))?$/.exec(path)[1];
          if (ext !== undefined){
            switch(ext.toLowerCase()){
              case "png":
              case "jpg":
              case "jpeg":
              case "gif":
              case "ico":
                return true;
            }
          }
          return false;
        }
        
        function fillFolderMenu(el, path){
          var list = document.createElement("ul");
          el.appendChild(list);
          var action = document.createElement("li");
          list.appendChild(action);          
          var isChecked = document.getElementById(path).childNodes[0].checked;
          var expnd = document.createElement("li");
          list.appendChild(expnd);
          if (isChecked){
            expnd.innerHTML = "<span>Collapse</span>";
            expnd.onclick = function(e){
              if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
              document.getElementById(path).childNodes[0].checked = false;
            };
            var refrsh = document.createElement("li");
            list.appendChild(refrsh);
            refrsh.innerHTML = "<span>Refresh</span>";
            refrsh.onclick = function(e){
              if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
              httpList(path);
            };
          } 
          else {
            expnd.innerHTML = "<span>Expand</span>";
            expnd.onclick = function(e){
              if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
              document.getElementById(path).childNodes[0].checked = true;
              httpList(path);
            };
          }
          var renFolder = document.createElement("li");
          list.appendChild(renFolder);
          renFolder.innerHTML = "<span>Rename/Move</span>";
          renFolder.onclick = function(e){
            if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
            var newPath = prompt("Rename " + path + " to", path);
            if (newPath != null && newPath != path) {
              httpRename(path, newPath);
            }
          };
          var delFolder = document.createElement("li");
          list.appendChild(delFolder);
          delFolder.innerHTML = "<span>Delete</span>";
          delFolder.onclick = function(e){
            if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
            httpDelete(path);
          };
        }
  
        function fillFileMenu(el, path){
          var list = document.createElement("ul");
          el.appendChild(list);
          var action = document.createElement("li");
          list.appendChild(action);
          if (isTextFile(path)){
            if (typeof ace == "undefined") {
              // Could not load editor
              action.innerHTML = "<span>View</span>";
              action.onclick = function(e){
                if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
                if (canLoadNewContents()) loadTxtPreview(path);
              };
            }
            else {
              action.innerHTML = "<span>Edit</span>";
              action.onclick = function(e){
                if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
                if (canLoadNewContents()) editor.loadUrl(path);
              };
            }
          } 
          else if (isImageFile(path)){
            action.innerHTML = "<span>Preview</span>";
            action.onclick = function(e){
              if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
              if (canLoadNewContents()) loadImgPreview(path);
            };
          }
          var download = document.createElement("li");
          list.appendChild(download);
          download.innerHTML = "<span>Download</span>";
          download.onclick = function(e){
            if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
            loadDownload(path);
          };
          var renFile = document.createElement("li");
          list.appendChild(renFile);
          renFile.innerHTML = "<span>Rename/Move</span>";
          renFile.onclick = function(e){
            if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
            var newPath = prompt("Rename " + path + " to", path);
            if (newPath != null && newPath != path) {
              httpRename(path, newPath);
            }
          };
          var delFile = document.createElement("li");
          list.appendChild(delFile);
          delFile.innerHTML = "<span>Delete</span>";
          delFile.onclick = function(e){
            if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(el);
            httpDelete(path);
          };
        }
  
        function showContextMenu(e, path, isfile){
          var divContext = document.createElement("div");
          var scrollTop = document.body.scrollTop ? document.body.scrollTop : document.documentElement.scrollTop;
          var scrollLeft = document.body.scrollLeft ? document.body.scrollLeft : document.documentElement.scrollLeft;
          var left = e.clientX + scrollLeft;
          var top = e.clientY + scrollTop;
          divContext.className = 'contextMenu';
          divContext.style.display = 'block';
          divContext.style.left = left + 'px';
          divContext.style.top = top + 'px';
          if (isfile) fillFileMenu(divContext, path);
          else fillFolderMenu(divContext, path);
          document.body.appendChild(divContext);
          var width = divContext.offsetWidth;
          var height = divContext.offsetHeight;
          divContext.onmouseout = function(e){
            if (e.clientX < left || e.clientX > (left + width) || e.clientY < top || e.clientY > (top + height)){
              if (document.body.getElementsByClassName('contextMenu').length > 0) document.body.removeChild(divContext);
            }
          };
        }
        
        function createTreeLeaf(parentPath, name, size){
          var leaf = document.createElement("li");
          leaf.id = ((parentPath == "/")?"":parentPath)+"/"+name;
          var span = document.createElement("span");
          if (isTextFile(name)) span.classList.add("txt");
          else if (isImageFile(name)) span.classList.add("img");
          span.innerHTML = name + " <i>(" + readableSize(size) + ")</i>";
          leaf.appendChild(span);
          leaf.onclick = function(e){
            attemptLoad(leaf.id);
          };
          leaf.oncontextmenu = function(e){
            e.preventDefault();
            e.stopPropagation();
            showContextMenu(e, leaf.id, true);
          };
          return leaf;
        }
  
        function createTreeBranch(parentPath, name, disabled){
          var leaf = document.createElement("li");
          leaf.id = ((parentPath == "/")?"":parentPath)+"/"+name;          
          var check = document.createElement("input");
          check.type = "checkbox";
          if (typeof disabled !== "undefined" && disabled) check.disabled = "disabled";
          leaf.appendChild(check);
          var label = document.createElement("label");
          label.textContent = name;
          leaf.appendChild(label);
          check.onchange = function(e){
            if (check.checked){
              httpList(leaf.id);
            }
          };
          label.onclick = function(e){
            if (!check.checked){
              check.checked = true;
              httpList(leaf.id);
            } 
            else {
              check.checked = false;
            }
          };
          leaf.oncontextmenu = function(e){
            e.preventDefault();
            e.stopPropagation();
            showContextMenu(e, leaf.id, false);
          }
          return leaf;
        }
  
        function addFileNodes(parentPath, items){
          items.sort(function(a, b) {
            if (a.type == b.type) {
              return a.name.localeCompare(b.name); // a before z
            }
            else {
              return a.type.localeCompare(b.type); // dir before file
            }           
          });
          var list = document.createElement("ul");

          document.getElementById(parentPath).appendChild(list);
          var ll = items.length;
          for(var i = 0; i < ll; i++){
            var item = items[i];
            var itemEl;
            if (item.type === "file") itemEl = createTreeLeaf(parentPath, item.name, item.size);
            else itemEl = createTreeBranch(parentPath, item.name);
            list.appendChild(itemEl);
          }   
        }
  
        this.attemptLoad = function(path) {
          console.log("Attempting load of '" + path  + "'...");
          document.getElementById("pathInput").value = path;
          if (canLoadNewContents()) {
            if (isTextFile(path)) {
              if (typeof ace == "undefined") {
                loadTxtPreview(path);
              }
              else {
                editor.loadUrl(path);              
              }
            }
            else if (isImageFile(path)) loadImgPreview(path);
            else clearMainPanel();
          }
        }
        
        /*
         * Refresh the given path, e.g. "/a/b/c/d"
         * It means we have to make sure d is already displayed
         * If not, we get to its parent first (c), if open, and so on (b, a).
         * Once we have found an ancestor, we refresh it, then we must come back to where we started, refreshing nodes on our way down.
         * This is done by pushing all paths to be refreshed in an array that will be pop'd in the callback function
         */
        this.refreshPath = function(path){
          if (fsInfo.type == "SPIFFS") {
            // on SPIFFS : No parent node => refresh full tree
            console.log("Refreshing '/'...");
            httpList("/");
          }
          else {
            console.log("Refreshing '" + path + "'...");
            if (path.lastIndexOf("/") == -1){
              // No "/" => reset the root
              httpList("/");
            } 
            else {
              var paths = [];            
              // Climb the tree until we get to an open node, adding paths to the array
              var parentPath = path;
              while ((parentPath.lastIndexOf("/") != -1) && !document.getElementById(parentPath)) {
                paths.push(parentPath);
                parentPath = getParentFolder(parentPath);
              }
              
              // If we've reached the top
              if (parentPath == "") paths.push("/"); // list the root
              else paths.push(parentPath); // otherwise list the last folder
              
              // And list it back down by iterating on collected paths
              listPaths(paths);  
            }
          }
        }

        
        //////////////////////////////
        // HTTP OPERATIONS
        
        // Callbacks
        
        function onListReceived(req, parentPath, remainingPaths) {
          return function(){
            setLoading(false);
            if (req.status != 200) showHttpError(req);
            else {
               // Remove previous child list, if any
              var parentEl = document.getElementById(parentPath);
              if (parentEl) {
                var lastChild = parentEl.lastElementChild;
                if (lastChild && lastChild.tagName == "UL") parentEl.removeChild(lastChild);
              }
              // And reinsert the received ones instead
              addFileNodes(parentPath, JSON.parse(req.responseText));
              if (document.getElementById(parentPath).childNodes[0].checked !== undefined) {
                document.getElementById(parentPath).childNodes[0].checked = true;              
              }
              // If there are more folders to refresh, go ahead
              if (remainingPaths && remainingPaths.length) {
                listPaths(remainingPaths);
              }
            }
          }
        }
  
        /*
         * Callback after a FS operation was performed. 
         * The "req" param is the http request
         * The "path" param is the "affected" (created, moved, renamed, delteted) path, of which the parent must be refreshed
         * In case of move/delete, the "target" path is returned in the response, so that it can be refreshed too
         * Operation      | path (file to be loaded) | req.responseText (between brackets if same as parent of path)
         * ---------------+--------------------------+--------------------------------------------------------------
         * Create file    | created file             | (parent of created file)
         * Create folder  | created folder           | (parent of created folder)
         * Rename file    | target file              | (parent of source file)
         * Move file      | target file              | parent of source file, or remaining ancestor
         ? Rename folder  | target folder            | (parent of source folder)
         ? Move folder    | target folder            | parent of source folder, or remaining ancestor
         * Delete file    |                          | parent of deleted file, or remaining ancestor
         * Delete folder  |                          | parent of deleted folder, or remaining ancestor
         */
        function onOperationComplete(req, path){
          return function(){
            setLoading(false);
            if (req.status != 200) showHttpError(req);
            else {
              if (path) {                
                var parentPath = getParentFolder(path);

                // Refresh returned path, if requested and different from path
                if (req.responseText && req.responseText != parentPath) {
                  refreshPath(req.responseText);
                }

                // Refresh original path
                refreshPath(parentPath);
                              
                // Try to load given path
                attemptLoad(path);
              }
              else {
                // Delete, only refresh returned path
                refreshPath(req.responseText);
              }              
            }
            refreshStatus();
          }
        }


        // Requests        
        
        function httpList(parentPath, remainingPaths){
          setLoading(true, "Listing '" + parentPath + "'...");         
          // Fetch an updated list
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = onListReceived(xmlHttp, parentPath, remainingPaths);
          xmlHttp.open("GET", "/list?dir="+parentPath, true);
          xmlHttp.send(null);
        }
        
        function listPaths(paths) {
          var path = paths.pop();     
          if (path) {
            httpList(path, paths);
          }
        }  

    
        this.httpUpload = function(file, path) {
          setLoading(true, "Uploading '" + path + "'...");
          if (!path.startsWith("/")) path = "/" + path;
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = onOperationComplete(xmlHttp, path);
          var formData = new FormData();
          formData.append("data", file, path);
          xmlHttp.open("POST", "/edit");
          xmlHttp.send(formData);
        };     

        this.httpCreate = function(path){
          setLoading(true, "Creating '" + path + "'...");
          if (!path.startsWith("/")) path = "/" + path;
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = onOperationComplete(xmlHttp, path);
          var formData = new FormData();
          formData.append("path", path);
          xmlHttp.open("PUT", "/edit");
          xmlHttp.send(formData);
        }
        
        function httpRename(srcPath, dstPath){
          setLoading(true, "Renaming '" + srcPath + "' to '" + dstPath + "'...");
          if (!dstPath.startsWith("/")) dstPath = "/" + dstPath;
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = onOperationComplete(xmlHttp, dstPath);
          var formData = new FormData();
          formData.append("path", dstPath);
          formData.append("src", srcPath);
          xmlHttp.open("PUT", "/edit");
          xmlHttp.send(formData);
        }

        function httpDelete(path) {
          setLoading(true, "Deleting '" + path + "'...");
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = onOperationComplete(xmlHttp);
          var formData = new FormData();
          formData.append("path", path);
          xmlHttp.open("DELETE", "/edit");
          xmlHttp.send(formData);
        }

  
        httpList("/");
        return this;
      }

      /////////////////////////
      // ACE EDITOR MANAGEMENT

      function createEditor(element, file, lang, theme, type){
        function getLangFromFilename(filename){
          var lang = "plain";
          var ext = /(?:\.([^.]+))?$/.exec(filename)[1];
          if (ext !== undefined){
            switch(ext){
              case "txt": lang = "plain"; break;
              case "htm": lang = "html"; break;
              case "js": lang = "javascript"; break;
              case "c": lang = "c_cpp"; break;
              case "cpp": lang = "c_cpp"; break;
              case "css":
              case "scss":
              case "php":
              case "html":
              case "json":
              case "xml":
                lang = ext;
            }
          }
          return lang;
        }
  
        if (typeof file === "undefined") file = "/index.htm";
  
        if (typeof lang === "undefined"){
          lang = getLangFromFilename(file);
        }
  
        if (typeof theme === "undefined") theme = "textmate";
  
        if (typeof type === "undefined"){
          type = "text/"+lang;
          if (lang === "c_cpp") type = "text/plain";
        }
  
        var xmlHttp = null;
        var editor = ace.edit(element);
        
        function filePosted(){
          setLoading(false);
          if (xmlHttp.status != 200) showHttpError(xmlHttp);
          tree.refreshPath(getParentFolder(file)); // to update size in tree
          refreshStatus();
        }
        function postFile(path, data, type){
          setLoading(true, "Saving '" + path + "'...");
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = filePosted;
          var formData = new FormData();
          formData.append("data", new Blob([data], { type: type }), path);
          xmlHttp.open("POST", "/edit");
          xmlHttp.send(formData);
        }

        function fileLoaded(){
          setLoading(false);
          document.getElementById("preview").style.display = "none";
          document.getElementById("editor").style.display = "block";
          document.getElementById("editorButtons").style.display = "inline";
          if (xmlHttp.status == 200) {
            editor.setValue(xmlHttp.responseText);
            editor.clearSelection();
            enableSaveDiscardBtns(false);
          }
          else tree.clearMainPanel();
        }
        function loadFile(path){
          setLoading(true, "Loading '" + path + "'...");
          xmlHttp = new XMLHttpRequest();
          xmlHttp.onload = fileLoaded;
          xmlHttp.open("GET", path, true);
          xmlHttp.send(null);
        }
  
        if (lang !== "plain") editor.getSession().setMode("ace/mode/"+lang);
        editor.setTheme("ace/theme/"+theme);
        editor.$blockScrolling = Infinity;
        editor.getSession().setUseSoftTabs(true);
        editor.getSession().setTabSize(2);
        editor.setHighlightActiveLine(true);
        editor.setShowPrintMargin(false);
        editor.commands.addCommand({
            name: 'save',
            bindKey: {win: 'Ctrl-S',  mac: 'Command-S'},
            exec: function(editor) {
              editor.save();
            },
            readOnly: false
        });
        editor.commands.addCommand({
            name: "showKeyboardShortcuts",
            bindKey: {win: "Ctrl-Alt-h", mac: "Command-Alt-h"},
            exec: function(editor) {
                editor.showShortcuts();
            }
        })

        editor.session.on('change', function(delta) {
          enableSaveDiscardBtns(true);
        });        

        editor.loadUrl = function(path){
          document.getElementById("pathInput").value = path;
          enableSaveDiscardBtns(false);
          file = path;
          lang = getLangFromFilename(file);
          type = "text/"+lang;
          if (lang !== "plain") editor.getSession().setMode("ace/mode/"+lang);
          loadFile(file);
        }
        
        editor.save = function() {
          enableSaveDiscardBtns(false);
          postFile(file, editor.getValue()+"", type);
        }
        
        editor.discard = function() {
          editor.loadUrl(file);
          enableSaveDiscardBtns(false);
        }
        
        editor.showShortcuts = function() {
          ace.config.loadModule("ace/ext/keybinding_menu", function(module) {
            module.init(editor);
            editor.showKeyboardShortcuts()
          });
        }

        loadFile(file);        
        
        return editor;
      }
      
      /////////////////////////
      // MAIN ENTRY POINT
      
      function onBodyLoad(){
        var vars = {};
        var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value) { vars[key] = value; });        
        if (typeof ace != "undefined") {
          var editor = createEditor("editor", vars.file, vars.lang, vars.theme);
        }
        tree = createTree("tree", editor);
        createHeader("header", tree, editor);
        refreshStatus();
      };
    </script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.9/ace.js" type="text/javascript" charset="utf-8"></script>   
    <script>
      if (typeof ace == "undefined") {
        console.log("Cannot load ace.js from the web, trying local copy");
        var script = document.createElement('script');
        script.src = "/edit/ace.js";
        script.async = false;
        document.head.appendChild(script);
      }
    </script>
  </head>
  <body onload="onBodyLoad();">
    <div id="header"></div>
    <div id="tree"></div>
    <div id="editor"></div>
    <div id="preview" style="display:none;"></div>
    <div id="loading"><span id="loading-msg"></span><br/><div class="spinner-anim">Loading...</div></div>
    <iframe id=download-frame style='display:none;'></iframe>
  </body>
</html>
 )rawliteral";
