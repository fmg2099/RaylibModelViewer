# Model Viewer

Este es un proyecto de ejemplo utilizando la biblioteca [raylib](https://www.raylib.com/) para crear un visor de modelos 3D. El proyecto incluye funcionalidades básicas para cargar texturas, dibujar objetos en 3D y controlar la cámara con el teclado y el ratón.

Basado en la plantilla [RaylibQuickstart](https://github.com/raylib-extras/raylib-quickstart).

## Características

- **Carga de Texturas:** Carga texturas desde un directorio de recursos y las aplica a objetos 3D.
- **Dibujo de Objetos 3D:** Dibuja cubos texturizados y esferas básicas en un entorno 3D.
- **Control de Cámara:** Controla la posición y orientación de la cámara utilizando el teclado y el ratón.
- **Skybox Simple:** Dibuja un fondo de gradiente para simular un cielo.
- **Interfaz de Usuario (HUD):** Muestra información de la cámara y los FPS en pantalla.
- **Efectos de postproceso** de cámara usando shaders.

## Controles

- **W/S:** Mover la cámara hacia adelante/atrás.
- **A/D:** Mover la cámara hacia la izquierda/derecha.
- **Q/E:** Mover la cámara hacia arriba/abajo.
- **Ratón:** Rotar la cámara.
- **R:** Reiniciar la posición de la cámara.

## Requisitos

- [raylib](https://www.raylib.com/) 3.7 o superior.
- Compilador compatible con C++14.

## Compilación y Ejecución

1. Clona el repositorio.
2. Ejecutar el script incluido (para Vs2022 o MinGW).
3. Compilar.