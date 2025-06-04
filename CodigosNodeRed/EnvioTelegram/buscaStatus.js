var data = flow.get("statusPlanta");
if (data === "perigo"){
    msg.payload = {
        chatId: "1331052919",
        type: "message",
        content: "Mensagem automática: A planta está em nivel de Perigo"
    }
    return msg;
}