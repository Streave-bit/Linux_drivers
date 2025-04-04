def main():
    driver_handle = open('/proc/hello_driver')
    message_from_driver = driver_handle.readline()
    print(message_from_driver)
    driver_handle.close()
    return

main()

