//O PROGMEM é um modificador de variável que armazena a string na memoria flash do arduino; a biblioteca desta função é a pgmspace.h, incluída automaticamente
//RAWLITERAL indica que se trata de uma String Literal, onde todos os caracteres são incluídos
 
 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <title>Index</title>
    <link href="css.css" rel="stylesheet" type="text/css">
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
    <span id="ShowInterval">
                
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
    <title>Get</title>
    <link href="css.css" rel="stylesheet" type="text/css">
</head>
<body>

  <h1>Novo intervalo de tempo definido</h1><br>
  <p>O intervalo de medição atual é:</p>
    <span id="ShowInterval">
        </span>        
            
  <br> <a href="/">Voltar à página inicial</a>

   <script type="text/javascript" src="script.js"></script>
</body>
</html>
)rawliteral";

const char css[] PROGMEM = R"rawliteral(
  /*Global*/

        *{
            /*Corrigindo comportamento padrão do css*/
            box-sizing: border-box;
        }
        
        html, body{
            margin: 0;
        }
        
        body{
            display: flex;
            flex-direction: column;
        
            height: 100vh;
        
            font-family: Arial, Helvetica, sans-serif;
        
            background-color: #f9f9f9 ;

            text-align: center;
            
        }
        
        h1{
            color: #000000cc;
        }

        /*HEADER*/
        header button{
            margin-top: 16px;
            padding: 9px 16px;
            border-radius: 4px;
            border: none;

            background-color: #50a8a1;
            color: white;

            font-weight: bold;

        }   
        

        header, .form{
            /*espaçamentos com margin*/
            margin: 0 auto 70px;
        
            /*limite dos elementos da página*/
            width: 90%;
            max-width: 500px;
        }

        
        /*FORM*/
        form{
            display: grid;
            grid-template-columns: 1fr 1fr;
            grid-gap: 45px;
        
            align-items: center;
        
            background-color: #54a086;
        
            padding: 16px 64px;
        
            border-radius: 6px;
        } 
        
        form input, form button{
        
            /*largura*/
            width: 110%;
        
            /*espaçamento*/
            margin-bottom: 4px;
        
            /*preechimento*/
            padding: 8px 16px;
        
            /*Bordas*/
            border-radius: 4px;
            border: 1px #ffffff88 solid;
        
        }
        
        form input{
            background-color: transparent;
        }
        
        form input::placeholder{
            color: #ffffffaa;
        }
        
        form button{
            font-weight: bold;
        }
        

        /*JVS*/
        section.form{
            transition: all 300ms;
        }
        
        section.form.hide{
            opacity: 0;
            visibility: hidden;
            height: 0;
            margin-top: -70px;   
        }

        span{
            font-size: 15pt;
            color: #54a086;
            font-weight: bolder;
        }

        a{
            text-decoration: default;
            margin-top: 15px;
            color: #54a086;
        }
        
)rawliteral";

const char script_interval[] PROGMEM = R"rawliteral(

            var showInterval = document.getElementById("ShowInterval");
var http = new XMLHttpRequest();

http.onreadystatechange = function(){ //é executada sempre que o status mudar
                
    if(this.status == 200){
        showInterval.innerHTML = detectUnit(this.responseText); //exibe a o texto recebido  
    }
};

    http.open("GET", "/interval", true); //metodo - url - async
    http.send();

function detectUnit(interval){
    interval = parseFloat(interval);
    console.log(interval);
    if(interval >= 3600) return `${interval/3600} hora(s)`
    else if(interval >= 60) return `${interval/60} minuto(s)`;
    else if(interval >= 1) return `${interval} segundo(s)`;
    else return;
}

        
)rawliteral";
