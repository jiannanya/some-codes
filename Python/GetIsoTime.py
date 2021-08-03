
def get_ISO_time():
    from datetime import datetime, timedelta, timezone
    tz_utc_8 = timezone(timedelta(hours=8))
    return  datetime.now(tz=tz_utc_8).isoformat()

if __name__ == '__main__':
    print(get_ISO_time())