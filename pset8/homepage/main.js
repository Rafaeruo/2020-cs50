var elements = ["home_title", "home_name", "home_quote", "home_parag", "home_link", "help"];
var counter = 0;

function conceal(){
    if (counter >= elements.length){
        return;
    }
    let element = "#"+elements[counter];
    document.querySelector(element).style.color = "white";
    counter += 1;
}