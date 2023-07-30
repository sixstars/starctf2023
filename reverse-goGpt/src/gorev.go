package main

import (
	"encoding/base64"
	"fmt"
	"math/rand"
	// "os"
)

const seed int64 = 34

func customEncrypt(flag []byte, key []byte) []byte {
	encrypted := make([]byte, len(flag))
	for i, b := range flag {
		encrypted[i] = b ^ key[i%len(key)]
	}
	return encrypted
}

func shuffle(key []byte) []byte {
	shuffledKey := make([]byte, len(key))
	copy(shuffledKey, key)

	rand.Seed(seed)
	for i := len(shuffledKey) - 1; i > 0; i-- {
		j := rand.Intn(i + 1)
		shuffledKey[i], shuffledKey[j] = shuffledKey[j], shuffledKey[i]
	}

	return shuffledKey
}

// if want to reverse
// func reverse(shuffledKey []byte) []byte {
// 	reversedKey := make([]byte, len(shuffledKey))
// 	copy(reversedKey, shuffledKey)

// 	rand.Seed(seed)
// 	for i := 1; i < len(reversedKey); i++ {
// 		j := rand.Intn(i + 1)
// 		reversedKey[i], reversedKey[j] = reversedKey[j], reversedKey[i]
// 	}

// 	return reversedKey
// }

func main() {
	// key := make([]byte, 16)
	key := []byte("cH@t_GpT_15_h3R3")
	shuffledKey := shuffle(key)

	//flag := []byte("*CTF{ch@tgpT_3nCRypt10n_4_FUN!!}")

	fmt.Printf("Input your flag:")

	var input_flag string
	fmt.Scanf("%s\n", &input_flag)

	if len(input_flag) != 32 {
		fmt.Println("Sorry, that's not the correct flag. Try again.")
		return
	}

	encryptedinput := customEncrypt([]byte(input_flag), shuffledKey)

	if base64.StdEncoding.EncodeToString(encryptedinput) == "fiAGBkgXN3McFy9hAHRfCwYaIjQCRDFsXC8ZYBFmEDU=" {
		fmt.Println("Congratulations! That's the correct flag!")
	} else {
		fmt.Println("Sorry, that's not the correct flag. Try again.")
	}
}
