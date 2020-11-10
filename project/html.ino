//O PROGMEM é um modificador de variável que armazena a string na memoria flash do arduino; a biblioteca desta função é a pgmspace.h, incluída automaticamente
//RAWLITERAL indica que se trata de uma String Literal, onde todos os caracteres são incluídos
 
 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <title>Index</title>
    <link href="css.css" rel="stylesheet">
</head>
<body>

    <h1>Informe o Intervalo de medição</h1>
    <header>
        <button id='show-setup-btn'>Informar dados</button>
    </header>

    <section class="form hide">
        <form action="/get">
            <input type="time" id="time" name="input1" min="00:00:15" step="1" required>         
            <input type="submit" value="Enviar">
            <input type="checkbox" name="sendNow" id="sendNow" value="1">
            <label for="sendNow">Enviar dados ao atualizar o intervalo</label>
        </form>
    </section>

    <p>O intervalo de medição atual é:</p>
    <span class="ti" id="ShowInterval"></span>

    <div class="feedback">    
        <p>Temperatura (°C): </p>
        <span class="ti" id="temperature">
        </span>

        <p>Umidade: </p>
        <span class="ti" id="humidity">
        </span>
    </div>

    <button type=button  id='restart-btn'>Reiniciar NodeMCU</button>

    <script>
        document.querySelector('header button#show-setup-btn')
         .addEventListener("click", function(){
            document.querySelector('.form').classList.toggle('hide');
        });

        document.querySelector('button#restart-btn').addEventListener('click', function(){
            var http = new XMLHttpRequest();
            http.open("GET", "/restart", true);
            http.send();
            window.alert("Reiniciado NodeMCU\nO portal de Configuração será inicializado\nEste servidor será encerrado");
        });
    </script>
    
    <script type="text/javascript" src="script.js"></script>

</body>
</html>
</html>
)rawliteral";

const char get_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <title>Novo Intervalo</title>
    <link href="css.css" rel="stylesheet">
</head>
<body>

  <h1>Novo intervalo de tempo definido</h1>

  <p>O intervalo de medição atual é:</p>

  <span class="ti" id="ShowInterval">
  </span>       
            
  <a href="/">Voltar à página inicial</a>

  <script type="text/javascript" src="script.js"></script>

</body>
</html>
)rawliteral";

const char css[] PROGMEM = R"rawliteral(
*{
    margin: 0;
    border: 0;
}

body{
    display: flex;
    justify-content: center;
    align-items: center;
    flex-direction: column;
}

h1, .ti{
    color: #52658C;
    font-weight: 700;
    padding: 20px;
    text-align: center;
}

.ti{
    font-size: 20pt;
}

p, header button{
    font-size: 15pt;
    font-weight: 600;
    color: #7685A8;
}

header button{
    padding: 15px;
    margin-bottom: 30px;
    border-radius: 20px;
}

form{
    border: 2px solid #7685A8;
    width: 200px;
    padding: 20px;
    border-radius: 20px;
    display: grid;
    grid-template-columns: 10% 90%;
    grid-template-areas:
                        "input1 input1"
                        "button button"
                        "check label";
    gap: 15px;
    justify-content: center;
    margin-bottom: 40px;
}

form *{
    text-align: center;
    font-weight: 600;
    font-size: 12pt;
    color: #7685A8;
    background-color: #F0F0F0;
    border-radius: 20px;
    height: 50px;
}

form input[name="input1"]{
    grid-area: input1;
}

form input[type="submit"]{
    grid-area: button;
}

form input[type='checkbox']{
    grid-area: check;

}

form label{
    grid-area: label;
    display: flex;
    align-items: center;
    justify-content: center;
    background: transparent;
}

section.form.hide{
    opacity: 0;
    visibility: hidden;
    height: 10px;
}

div.feedback {
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
}

a{
    text-decoration: none;
    background-color: #F0F0F0;
    color: #7685A8;
    padding: 15px;
    border-radius: 20px;
}

button#restart-btn {
    margin-top: 75px;
    padding: 13px;
    background: red;
    border-radius: 8px;
    color: white;
    font: 800 normal 12pt Arial
}

)rawliteral";

const char script_interval[] PROGMEM = R"rawliteral(
  function detectUnit(interval){
  interval = parseInt(interval);
  interval = interval / 1000;
  console.log(interval);
  var hrs = Math.floor(interval / (60 * 60));
  var mins = Math.floor(interval % (60 * 60) / 60);
  var sec = Math.ceil((interval % (60 * 60)) % 60);

  if(hrs < 10) hrs = `0${hrs}`;
  if(mins < 10) mins = `0${mins}`;
  if(sec < 10) sec = `0${sec}`;
  
  return `${hrs}:${mins}:${sec}`;
}

var ihttp = new XMLHttpRequest();
ihttp.onreadystatechange = function(){ //é executada sempre que o status mudar
    if(this.status == 200){
        document.getElementById("ShowInterval").innerHTML = detectUnit(this.responseText);  
    }
};

ihttp.open("GET", "/interval", true); //método - url - async
ihttp.send();


//temperatura
setInterval(function(){
    var http = new XMLHttpRequest();

    http.onreadystatechange = function() {
      if (this.status == 200) {
        document.getElementById("temperature").innerText = this.responseText;
      }
    };

    http.open("GET", "/temperature", true);
    http.send();

    }, 1000);

//umidade
setInterval(function(){
    var http = new XMLHttpRequest();

    http.onreadystatechange = function() {
      if (this.status == 200) {
        document.getElementById("humidity").innerText = this.responseText;
      }
    };

    http.open("GET", "/humidity", true);
    http.send();

    }, 1000);

)rawliteral";
