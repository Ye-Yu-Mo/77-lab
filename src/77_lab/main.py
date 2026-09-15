def bubble_sort(numbers: list[int]) -> list[int]:
    """使用冒泡排序将列表按升序排列。"""
    numbers = numbers.copy()

    for end in range(len(numbers) - 1, 0, -1):
        swapped = False
        for i in range(end):
            if numbers[i] > numbers[i + 1]:
                numbers[i], numbers[i + 1] = numbers[i + 1], numbers[i]
                swapped = True

        if not swapped:
            break

    return numbers


if __name__ == "__main__":
    print(bubble_sort([5, 2, 8, 1, 3]))
