// script.js

let butto = document.querySelector("#btn-id")
let text = document.querySelector("#text-area")

// eventListener "click" on button
butto.addEventListener("click", () => {
    let valueinput = text.value

    let blobdtMIME =
        new Blob([valueinput], { type: "text/plain" })
    let url = URL.createObjectURL(blobdtMIME)
    let anele = document.createElement("a")
    anele.setAttribute("download", "Downloaded Successfully");
    anele.href = url;
    anele.click();
    console.log(blobdtMIME)
})