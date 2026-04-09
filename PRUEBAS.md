# Pruebas inmediatas

1. Eric aparece en pantalla 1 de Prehistoria (cima de la colina) con fondo de color marrón y Eric como rectángulo amarillo
2. Eric puede moverse a la derecha hasta el borde y pasar a pantalla 2
3. El fade funciona al cambiar de pantalla
4. Eric aparece en el lado correcto de la pantalla destino

## Pruebas medio plazo
5. Verificar el mapa completo de navegación
Recorrer las 9 pantallas de Prehistoria en todas las direcciones y verificar que las conexiones son correctas según el GDD. Especialmente las conexiones verticales (liana) entre pantalla 4 y 7.
6. Verificar los bloqueos
Comprobar que Eric no puede pasar de pantalla 8 a pantalla 9 en Prehistoria (río bloqueado hasta resolver PUZZLE_RIVER). Esto verificará que screen_get_connection funciona correctamente.
7. Verificar el viaje temporal
Probar que al pulsar ALT Eric viaja a Edad Media y Futuro manteniendo la misma pantalla, y que el fade funciona correctamente.
8. Verificar la lógica básica de un puzzle
Implementar y probar el puzzle más simple: el oso y la miel. Esto verificará que logic_update, inv_pick, inv_transform y puzzle_solve funcionan juntos correctamente.
9. Verificar el guardado
Guardar en la pantalla 5 y comprobar que al reiniciar el juego carga correctamente en esa pantalla con el estado correcto.
10. Integrar el primer BMP de fondo
Cuando tu colaboradora entregue el primer fondo, integrarlo y verificar que draw_bitmap_buf lo muestra correctamente y que la paleta no rompe los colores del sprite.
El orden importa: cada paso depende del anterior. No tiene sentido probar puzzles si la navegación no funciona, ni integrar arte si los puzzles no funcionan.

## Pruebas completar el juego jugable
11. Implementar y probar todos los puzzles uno a uno
Siguiendo el orden del GDD: puzzle simple de Prehistoria, luego el medio, luego el principal. Cada puzzle verificado antes de pasar al siguiente.
12. Integrar arte progresivamente
A medida que tu colaboradora entregue assets, integrarlos pantalla a pantalla. No esperar a tener todo el arte para integrar.
13. Implementar el rebobinado
Es una mecánica compleja que afecta al estado global del juego. Necesita su propio sistema con un buffer de estados anteriores.
14. Implementar la secuencia del Mecha
Es la secuencia más compleja técnicamente: cambio de control, HUD especial, contador del cristal, conducción entre pantallas.
15. Pantallas de título y fin
Con arte final.
16. Música por época
Cuando tu colaboradora entregue los XM, cargar la música correcta al cambiar de época

## Pulido
17. Ajustar dificultad arcade
Velocidades de enemigos, timing de peces, distancia de interacción. Todo basado en pruebas reales.
18. Ajustar timing del cristal del Mecha
El valor de CRISTAL_CARGA_TICKS que definimos como ajustable.
19. Prueba completa de principio a fin
Completar el juego entero siguiendo la solución del GDD y verificar que no hay estados sin salida.

## Largo plazo: preparación para el concurso
20. Hacer público el repositorio
21. Crear ejecutable final
22. Escribir el README definitivo
23. Enviar al concurso antes del 30 de septiembre
.