# TIT Q v0.5

Ecualizador gratuito de 10 bandas con dinámica por banda, sidechain y curvas de carácter propio. Formatos: **VST3 (Windows y macOS)** y **AU (macOS)**. Sin AAX, sin Standalone.

## Fase lineal
- Selector **Phase: Min / Linear / Mixed** en la barra superior. En *Min* el ecualizador funciona como siempre (con oversampling opcional). En *Linear* las 10 bandas se convierten en un filtro FIR simétrico, sin desfase, y el plugin avisa de su latencia al host. En *Mixed* (fase mixta) el filtro tiene la fase mínima del ecualizador por debajo de ~500 Hz y fase lineal por encima: graves sin pre-ringing y agudos sin desfase; la magnitud es idéntica y la latencia, la misma que en *Linear*.
- **Calidad** (Low / Medium / High / Max): longitud del filtro. A 48 kHz la latencia es de 1280 / 2304 / 4352 / 8448 muestras (a 96 kHz y 192 kHz la longitud se duplica y cuadruplica para mantener la misma resolución en Hz). Más calidad = más precisión en graves y más latencia.
- Funcionan igual que en fase mínima: todos los tipos de banda y modos de carácter (se conserva su magnitud), pendientes continuas, dinámica por banda (el filtro se recalcula en otro hilo y se cambia con un fundido, sin clics), sidechain, Mid/Side/L/R, solo por banda y la curva dibujada. En *Linear* el oversampling no hace falta: la respuesta se calcula a 4x, sin la deformación del bilineal cerca de Nyquist.
- Coste: los filtros lineales producen *pre-ringing* en cortes pronunciados y realces graves (es inherente a la fase lineal). En los graves más extremos la precisión depende de la calidad (resolución = frecuencia de muestreo / longitud del filtro).
- Un render fuera de tiempo real diseña el filtro en el propio hilo de audio, así el resultado no depende de la velocidad del equipo.

## Novedades v0.5
- **TIT 250** (antes BIG TIT): campana con Q ajustable entre 0.393 (más ancha) y 2.121 (más estrecha), medidas en Plugin Doctor.
- **TIT SKY**: high shelf de 12 dB/oct cuya frecuencia y Q siguen las curvas medidas al subir los dB (2.6 dB: 11831.67 Hz, Q 0.499; 4.44 dB: Q 0.457; 6.84 dB: 10388.33 Hz; 8.2 dB: 10053.76 Hz; tope 10.14 dB: 9624.37 Hz, Q 0.475). Por defecto arranca en 11831.67 Hz y la frecuencia elegida se desplaza con esas curvas.
- **BOOBTEC** con pasos fijos de graves, con las frecuencias reales medidas: 25, 45, 89.18 y 136.94 Hz (posiciones 20, 30, 60 y 100 del original).
- **Fase mixta** (Phase: Mixed): mínima en graves, lineal en agudos.
- Motor: los coeficientes de la dinámica se interpolan muestra a muestra (antes saltaban cada 32 muestras); campanas y shelves cerca de Nyquist se diseñan contra el prototipo analógico sin necesidad de oversampling (se elige el diseño más fiel); las 10 bandas se encadenan en 64 bits y solo se redondea a 32 bits a la salida.

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
