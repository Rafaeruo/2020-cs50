

function check(){
    if (document.querySelector("#answer").value.toLowerCase() == "pissed duck"){
        alert("That's right, the duck indeed feels PISSED!")
        return 0
    }
    alert("Nope, that's not how the duck feels. :(")
    return 0
}

saturation_slider = document.querySelector("#saturation_slider")
contrast_slider = document.querySelector("#contrast_slider")
brightness_slider = document.querySelector("#brightness_slider")

function update(){
    let sat = saturation_slider.value
    let con = contrast_slider.value
    let bri = brightness_slider.value
    document.querySelector("#saturation_value").innerHTML = "Saturation: "+sat
    document.querySelector("#contrast_value").innerHTML = "Contrast: "+con+"%"
    document.querySelector("#brightness_value").innerHTML = "Brightness: "+bri+"%"

    document.querySelector("img").style.filter = "saturate("+sat+") contrast("+con+"%) brightness("+bri+"%)"
    
}