import numpy as np

vocabulary_file = "word_embeddings.txt"

# Read words
print("Read words...")
with open(vocabulary_file, "r", encoding="utf-8") as f:
    words = [x.rstrip().split(" ")[0] for x in f.readlines()]

# Read word vectors
print("Read word vectors...")
with open(vocabulary_file, "r", encoding="utf-8") as f:
    vectors = {}
    for line in f:
        vals = line.rstrip().split(" ")
        vectors[vals[0]] = [float(x) for x in vals[1:]]

vocab_size = len(words)
vocab = {w: idx for idx, w in enumerate(words)}
ivocab = {idx: w for idx, w in enumerate(words)}

vector_dim = len(vectors[ivocab[0]])
W = np.zeros((vocab_size, vector_dim))
for word, v in vectors.items():
    if word == "<unk>":
        continue
    W[vocab[word], :] = v

while True:
    dist_dict = {}
    dist_dict_len = {}

    words = input("Enter several words separated by comma: ").split(",")
    word_comb_k = [i.strip() for i in words]

    try:
        W_diff = W[vocab[word_comb_k[0]]]

        for i in range(1, len(word_comb_k)):
            W_diff = W_diff - W[vocab[word_comb_k[i]]]

    except KeyError as z:
        print(z, " word is not found, check spelling")

    else:

        dist_dict_k = {}
        dist_arr = []

        for i in vocab:

            x = W[vocab[i]] + W_diff  # compare b+c-a instead of a-b-c
            dist = -np.sum(x * x)

            # cosine distance
            #dist =  -np.sum(W[vocab[i]]*W_diff)/(np.sum(W[vocab[i]]*W[vocab[i]])*np.sum(W_diff*W_diff))**(0.5)

            if i not in word_comb_k:
                dist_dict_k.update({dist: i})
                dist_arr.append(dist)

        # for usual distanse      
        #dist_arr = np.sort(np.array(dist_arr))[0:2]

        # for cosine simularity
        dist_arr = np.sort(np.array(dist_arr))[-3:-1]

        words_list = ", ".join([dist_dict_k[i] for i in dist_arr]) 

        word_comb_k_str = "\"" + word_comb_k[0] + "\"" + \
            " is related to " + "\"" + word_comb_k[1] + "\"" + \
            " as " + "\"" + word_comb_k[2] + "\"" + " related to "

        dist_dict.update({word_comb_k_str: words_list})
        dist_dict_len.update({word_comb_k_str: dist_arr})

        print(word_comb_k_str, "\"" + words_list+"\"",  " with distances: ",
                ", ".join([str(round(j, 4)) for j in dist_arr]))

       