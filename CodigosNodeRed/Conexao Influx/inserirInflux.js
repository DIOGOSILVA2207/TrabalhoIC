msg.payload = [{
    measurement: "plantBot",
    fields: {
        temperature: msg.payload.temperatura,
        humidadeSolo: msg.payload.humidade_solo,
        humidadeAr: msg.payload.humidade,
        luminosidade: msg.payload.luminosidade,
        nivel_luminosidade: msg.payload.nivel_luminosidade,
        solo: msg.payload.solo,
        estado: msg.payload.estado_planta
    },
    tags: {
        sensorID: "1",
        location: "planta1"
    }
}
];
return msg;