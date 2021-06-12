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

    <h1>Informe o Intervalo de Medição</h1>
    <button id='show-setup-btn'>Informar dados</button>

    <section class="form hide">
        <form action="/get">
            <spam id="hms">Horas : Minutos : Segundos</spam>
            <input type="time" value="00:00:00" id="time" name="input1" min="00:00:15" step="1" required>         
            <input type="submit" value="Enviar">
            <div>
                <input type="checkbox" name="sendNow" id="sendNow" value="1">
                <label for="sendNow">Enviar dados ao atualizar o intervalo</label>
            </div>
        </form>
    </section>

    
    <p>O intervalo de medição atual é:</p>
    <span class="ti" id="ShowInterval"></span>

    <section id="feedback">
        <p>Temperatura (°C): </p>
        <span class="ti" id="temperature">
        </span>

        <p>Umidade (%): </p>
        <span class="ti" id="humidity">
        </span>

        <p>Luminosidade (lux): </p>
        <span class="ti" id="luminosity">
        </span>
    </section>

    <button type=button  id='restart-btn'>Reiniciar NodeMCU</button>

    <script>
        document.querySelector('button#show-setup-btn')
         .addEventListener("click", function(){
            document.querySelector('.form').classList.toggle('hide');
        });

        document.querySelector('button#restart-btn').addEventListener('click', function(){
            if (window.confirm("Esta opção irá reiniciar o NodeMCU e todo o seu servidor será resetado.\nTem certeza que deseja continuar?")){
                var http = new XMLHttpRequest();
                http.open("GET", "/restart", true);
                http.send();
            }
        });
    </script>
    
    <script type="text/javascript" src="script.js"></script>
</body>
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

h1{
    color: #52658C;
    font-weight: 700;
    padding: 20px;
    text-align: center;
}

p, #show-setup-btn{
    font-size: 15pt;
    font-weight: 600;
    color: #7685A8;
}

#show-setup-btn{
    padding: 15px;
    margin-bottom: 30px;
    border-radius: 8px;
}

section.form.hide{
    opacity: 0;
    visibility: hidden;
    height: 10px;
}

#feedback{
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    margin-top: 15px;
}

.ti{
    font-size: 20pt;
    color: #52658C;
    text-align: center;
    margin-bottom: 15px;
    font-weight: 700;
   
}

button#restart-btn {
    margin: 50px;
    padding: 15px;
    background: red;
    border-radius: 8px;
    color: white;
    font: 800 normal 12pt Arial;
}

form{
    border: 2px solid #7685A8;
    padding: 20px;
    border-radius: 8px;

    margin-bottom: 30px;
}

.form form{
    display: inline-flex;
    flex-direction: column;
    align-items: center;
    gap: 15px;
    position: relative;
}

form *{
    width: 90%;
    font-weight: 600;
    color: #7685A8;
    text-align: center;
}

.form form div{
    display: flex;
}

.form form div label{
    min-width: 80%;
}

form input[type="submit"], #time{
    font-size: 12pt;
    background-color: #F0F0F0;
    border-radius: 8px;
    min-height: 50px;
}

#hms{
    color: white;
    background-color: #7685A8;
    padding: 5px 2px 5px 2px;
    border-radius: 8px;
    position: absolute;
    width: 50%;
    font-size: 9pt;
    top: -11px;
}

a{
    text-decoration: none;
    background-color: #F0F0F0;
    color: #7685A8;
    padding: 15px;
    border-radius: 8px;
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

var http = new XMLHttpRequest();
  http.onreadystatechange = function(){ //é executada sempre que o status mudar
      if(this.status == 200){
          document.getElementById("ShowInterval").innerHTML = detectUnit(this.responseText);
      }
  };

  http.open("GET", "/interval", true); //método - url - async
  http.send();

function sendRequest(variable){
  http.onreadystatechange = function() {
    if (this.status == 200) {
      document.getElementById(variable).innerText = this.responseText;
    }
  };
  http.open("GET", "/" + variable, false);
  http.send();
}

setInterval(function(){
    sendRequest("temperature");
    sendRequest("humidity");
    sendRequest("luminosity");
}, 1000);
)rawliteral";
