# TIT Q v0.4

Ecualizador gratuito de 10 bandas con dinámica por banda, sidechain y curvas de carácter propio. Formatos: **VST3 (Windows y macOS)** y **AU (macOS)**. Sin AAX, sin fase lineal, sin Standalone.

## Novedades v0.4
- TITTEC pasa a llamarse **BOOBTEC**; sus mandos son **BOOB** (antes Boost) y **ATTIT** (antes Atten).
- **TIT SKY** actualizado: high shelf de 12 dB/oct con Q 0.475; al elegirlo, la frecuencia arranca en 9107.48 Hz y se puede mover.
- Al elegir BOOBTEC, la frecuencia arranca en 10.3 Hz y se puede mover.
- Rótulos y botones con cajas casi transparentes y letra clara para leerse sobre el fondo.

## Novedades v0.3
- Eje de frecuencias hasta 21k (etiquetas hasta 20k y 21k, notación k desde 1000 Hz).
- Curva de Low Pass corregida (ya no sube hacia 0 dB en agudos).
- Pendientes continuas de 1 dB/oct sin clics. Las 8 secciones salen de una tabla ajustada por mínimos cuadrados a un Butterworth continuo: error máximo medido 0.73 dB frente a la curva ideal, y -3 dB exactos en fc. El corte más suave es 6 dB/oct (valores menores se tratan como 6).
- Inicio limpio: sin bandas. Un solo clic en una zona vacía de la gráfica crea la banda en el primer hueco libre (1 a 10) y se puede arrastrar al instante. Doble clic sobre una banda la enciende/apaga.
- Clic en graves (20-35 Hz) crea High Pass; en agudos (17k-21k) crea Low Pass.
- Decimales: Freq 2, Q 3, Attack 2, Release 2, resto 3 como máximo.
- Logo TIT Q más arriba y algo mayor, con la Q dibujada.
- AP TIT: Bell con Q automática (2.10 dB→0.332, 4.03→0.435, 6.10→0.487, 8.90→0.565, 12.00→0.608; usa |dB|).
- Botón Settings: acción de clics y dobles clics, presets .json en `Documents/TIT Q/Presets`, marcar uno como Default (se carga al abrir el plugin; el inicio sigue sin bandas).

## Compilar
Sube todos los archivos a la raíz del repositorio y `build.yml` a `.github/workflows/build.yml`. Borra los archivos antiguos. El workflow genera el artefacto `TITQ-all`.
macOS sin firmar: `xattr -cr TIT\ Q.vst3` (o .component).

Licencia: JUCE 8 es AGPLv3, añade un archivo LICENSE.

## Licencia

TIT Q es gratuito y se distribuye bajo la licencia GNU AGPLv3 (ver el archivo LICENSE).
Está construido con JUCE 8. El código fuente está disponible en:
https://github.com/apipe360-ship-it/TIT-Q
