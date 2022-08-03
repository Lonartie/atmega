from Window import Window


def main():
    """
    Runs the application
    """
    window = Window()
    if window.valid:
        window.run()
    window.close()


if (__name__ == '__main__'):
    main()
