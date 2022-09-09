Process to get the measurements from the sensor:

- set WriteMode to enabled by sending A01F to the sensor (via characteristic 1a00 and service 1204)
- split returned 16 bytes value according to schema, see e.g. https://github.com/basnijholt/miflora) (via characteristic 1a01 and service 1204)
