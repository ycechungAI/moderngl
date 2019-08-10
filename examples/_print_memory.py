def print_memory(data, rows, spaces):
    s = len(data) // rows
    print('\n'.join(
        ' '.join(data[i + k * s : j + k * s].hex() for i, j in zip([0] + spaces, spaces + [s]))
        for k in range(rows)
    ))
