const { createBluetooth } = require('node-ble')
const { bluetooth, destroy } = createBluetooth()


const SERVICE_UUID = "00001204-0000-1000-8000-00805f9b34fb"
const CMD_CHAR = "00001a00-0000-1000-8000-00805f9b34fb"
const DATA_CHAR = "00001a01-0000-1000-8000-00805f9b34fb"


const DATA_MODE_CHANGE = Buffer.from('A01F', 'hex');

async function main() {
    const adapter = await bluetooth.defaultAdapter()
    if (! await adapter.isDiscovering())
        await adapter.startDiscovery()

    const device = await adapter.waitDevice('C4:7C:8D:6D:36:2D')
    await device.connect()
    const gattServer = await device.gatt()

    const service1 = await gattServer.getPrimaryService(SERVICE_UUID)

    const characteristic1 = await service1.getCharacteristic(DATA_CHAR)
    const characteristic2 = await service1.getCharacteristic(CMD_CHAR)

    await characteristic2.writeValue(DATA_MODE_CHANGE, { offset: 0, type: 'request' })
    const buffer = await characteristic1.readValue()
    console.log(buffer)

    await device.disconnect()
    destroy()
}

main();