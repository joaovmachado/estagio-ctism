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
        <button>Informar dados</button>
    </header>

    <section class="form hide">

        <form action="/get">
            <input type="number" min=1 step="0.01" value="1" name="input1" placeholder="Intervalo de medição" required>
            <select name="unit">
                <option value="3600" selected>Horas</option>
                <option value="60">Minutos</option>
                <option value="1">Segundos</option>
            </select>
            <input type="submit" value="Enviar">
        </form>

    </section>
        
    <p>O intervalo de medição atual é:</p>
    <span class="ti" id="ShowInterval">
    </span>

    
    <p>Temperatura (°C): </p>
    <span class="ti" id="temperature">
    </span>

    <p>Umidade: </p>
    <span class="ti" id="humidity">
    </span>

    <script>
        document
        .querySelector('header button')
        .addEventListener("click", function(){
        document
        .querySelector('.form')
        .classList.toggle('hide');
        });
    </script>
    
    <script type="text/javascript" src="script.js"></script>

</body>
</html>
</html>)rawliteral";

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
</html>)rawliteral";

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
        padding: 25px;
        border-radius: 20px;
        display: grid;
        grid-template-columns: 50% 50%;
        grid-template-areas:
                            "input select"
                            "button button";
        gap: 20px;
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
    
    form input[type="number"]{
        grid-area: input;
    }
    
    form select{
        grid-area: select;
    }
    
    form input[type="submit"]{
        grid-area: button;
    }
    
    section.form.hide{
        opacity: 0;
        visibility: hidden;
        height: 0;
    }
    
    a{
        text-decoration: none;
        background-color: #F0F0F0;
        color: #7685A8;
        padding: 15px;
        border-radius: 20px;
    }  
)rawliteral";

const char script_interval[] PROGMEM = R"rawliteral(
  function detectUnit(interval){
      interval = parseFloat(interval);
      console.log(interval);
      if(interval >= 3600) return `${(interval/3600).toFixed(2)} hora(s)`
      else if(interval >= 60) return `${(interval/60).toFixed(2)} minuto(s)`;
      else if(interval >= 1) return `${interval} segundo(s)`;
      else return;
  }
  
  var ihttp = new XMLHttpRequest();
  ihttp.onreadystatechange = function(){ //é executada sempre que o status mudar
                  
      if(this.status == 200){
          document.getElementById("ShowInterval").innerHTML = detectUnit(this.responseText); //exibe a o texto recebido  
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
