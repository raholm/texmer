#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>

#include "lexical_evaluator.h"

#include "block_evaluator.h"
#include "vocabulary_evaluator.h"

#include "token_sequence.h"
#include "topic_indicator_sequence.h"

using TextTileBlockEvaluator = BlockEvaluator<TokenSequence, std::string>;
using TopicTileBlockEvaluator = BlockEvaluator<TopicIndicatorSequence, std::size_t>;

using TextTileVocabularyEvaluator = VocabularyEvaluator;

#endif // EVALUATOR_H
