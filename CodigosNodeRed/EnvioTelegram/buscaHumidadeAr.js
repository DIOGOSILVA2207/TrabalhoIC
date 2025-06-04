// from(bucket: "TP")
//   |> range(start: -5m)
//   |> filter(fn: (r) => r["_measurement"] == "plantBot" and
//                     r["_field"] == "humidadeAr" and
//                     r["location"] == "planta1")
//   |> last()