var imageDataURL;
function imageToURL(input){
    var reader = new FileReader();
    reader.onload = function(e){//called as soon as readAsDataURL is called
        imageDataURL = e.target.result;//saves the dataURL to the var
        document.querySelector("#img_read").src = imageDataURL;//change html #img_read source to the dataurl
    }
    reader.readAsDataURL(input.files[0])
    document.querySelector("#warning_1").innerHTML = "This is the image you just selected!"
    //clean:
    document.querySelector("#message").innerHTML = "Decoded message: ";
}

function getInput(){
    if (document.querySelector("#secret_message").value == "" || imageDataURL == null){
        alert("Select and image and type in a secret message first!")
        return;
    }
    var encoded = steg.encode(document.querySelector("#secret_message").value, imageDataURL);//saves the encoded dataURL to encoded var
    document.querySelector("#img_encoded").src = encoded;//change html #img_encoded source to the encoded dataURL
    document.querySelector("#download").href = encoded;
    document.querySelector("#download").innerHTML = "Download";
    document.querySelector("#warning_2").innerHTML = "It looks the same because the message is hidden.";

    //clean:
    document.querySelector("#secret_message").value = "";
    document.querySelector("#message").innerHTML = "Decoded message: ";
}

function decode(){
    if (document.querySelector("#img_encoded").src == ""){
        alert("Encode a secret message into an image first!");
        return;
    }
    var text = steg.decode(document.querySelector("#img_encoded").src);
    document.querySelector("#message").innerHTML = "Decoded message: "+text;
}

var proofImageDataURL;
function getProofInput(input){
    var reader = new FileReader();
    reader.onload = function(e){
        proofImageDataURL = e.target.result;
        document.querySelector("#img_proof").src = proofImageDataURL;
    }
    reader.readAsDataURL(input.files[0])
    //clean:
    document.querySelector("#proof_message").innerHTML = "Decoded message: ";
}

function decodeProof(){
    if (proofImageDataURL == null){
        alert("Select an image first!");
        return;
    }
    let proof_text = steg.decode(proofImageDataURL);
    document.querySelector("#proof_message").innerHTML = "Decoded message: "+proof_text;
}