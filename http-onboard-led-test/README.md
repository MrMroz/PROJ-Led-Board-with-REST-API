### Test biblioteki picow-http

Na podstawie [Przykładowego projektu](https://gitlab.com/slimhazard/picow-http-example/-/tree/master/)
Na **gpio 0** ma być led podpięty

Zdefiniowane dwa endpointy:
- `/led/ (GET)`
- `/led/ (PUT)`

`GET /led/` ma zwracać stan led'a (OFF/ON)
`PUT /led/` zmienia stan za pomocą query param (`/led?state=on` lub  `/led?state=off`)
