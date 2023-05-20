def calculate_last_sunday_in_month(year, month):
    if month == 2:
        # February
        if (year % 4 == 0 and year % 100 != 0) or year % 400 == 0:
            d = 29  # Leap year
        else:
            d = 28  # Non-leap year
    elif month == 4 or month == 6 or month == 9 or month == 11:
        # April, June, September, November
        d = 30
    else:
        # January, March, May, July, August, October, December
        d = 31

    K = year % 100
    J = year // 100

    w = (d + 13 * (month + 1) // 5 + K + K // 4 + J // 4 + 5*J) % 7

    diff = w - 1;  # Calculate the difference from 0 (Sunday)

    lastSunday = d - diff

    return lastSunday



def calculate_first_sunday_in_month(year, month):
    
    if (month == 1) :
        month = 13
        year = year - 1
 
    if (month == 2) :
        month = 14
        year = year - 1

    d = 1
    K = year % 100
    J = year // 100

    w = (d + 13 * (month + 1) // 5 + K + K // 4 + J // 4 + 5*J) % 7
    d = ((w+5)%7) + 1 #convert so monday=1, sunday=7

    first_sunday = 1 if d == 7 else 8 - d

    return first_sunday

# Example usage:
year = 2023
month = 7
last_sunday = calculate_last_sunday_in_month(year,month)
print(f"The last Sunday in {month} / {year} is on the {last_sunday}th.")


first_sunday = calculate_first_sunday_in_month(year,month)
print(f"The first Sunday in {month} / {year} is on the {first_sunday}th.")
print(f"The second Sunday in {month} / {year} is on the {first_sunday+7}th.")