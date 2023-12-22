import numpy as np

vocabulary_file = 'word_embeddings.txt'

# Read words
print('Read words...')
with open(vocabulary_file, 'r', encoding='utf-8') as f:
    words = [x.rstrip().split(' ')[0] for x in f.readlines()]

# Read word vectors
print('Read word vectors...')
with open(vocabulary_file, 'r', encoding='utf-8') as f:
    vectors = {}
    for line in f:
        vals = line.rstrip().split(' ')
        vectors[vals[0]] = [float(x) for x in vals[1:]]

vocab_size = len(words)
vocab = {w: idx for idx, w in enumerate(words)}
ivocab = {idx: w for idx, w in enumerate(words)}

vector_dim = len(vectors[ivocab[0]])
W = np.zeros((vocab_size, vector_dim))
for word, v in vectors.items():
    if word == '<unk>':
        continue
    W[vocab[word], :] = v

# words = ["king", "europe", "frog", "bicycle", "countires", "foods"]

while True:

    dist_dict = {}
    dist_dict_len = {}

    words = input("Enter one or several words separated by comma: ").split(",")
    words = [i.strip() for i in words]

    for word_curr in words:

        dist_dict_k = {}
        dist_arr = []

        try:
            vect_word = W[vocab[word_curr]]
        except KeyError as z:
            print(z, " word is not found, check spelling")
        else:
            for i in vocab:

                x = (W[vocab[i]] - vect_word)
                dist = np.sum(x*x)

                dist_arr.append(dist)
                dist_dict_k.update({dist: i})

                #if dist in dist_dict_k:
                #    dist_dict_k[dist].append(i)
                #else:
                #    dist_arr.append(dist)
                #    dist_dict_k.update({dist: [i]})

            dist_arr = np.sort(np.array(dist_arr))[0: 4]
            #words_list = [
            #    ", ".join([j for j in dist_dict_k[i]]) for i in dist_arr
            #    ]
            
            words_list = [dist_dict_k[i] for i in dist_arr]
            
            dist_dict.update({word_curr: words_list})
            dist_dict_len.update({word_curr: dist_arr})

        for i in dist_dict:
                        
            print(f"Closest words for \"{i}\" are: \"" +
                  (", ".join([str(j) for j in dist_dict[i]])) +
                  "\"", "with distances: ",
                  ", ".join([str(round(j, 4)) for j in dist_dict_len[i]]))

