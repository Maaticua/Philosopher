#!/bin/bash

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# ============================================
# CRÉER LE FICHIER DE SUPPRESSION PTHREAD
# ============================================
create_suppression_file() {
    cat > pthread.supp << 'EOF'
{
   pthread_TLS_possibly_lost
   Memcheck:Leak
   match-leak-kinds: possible
   ...
   fun:_dl_allocate_tls
}
{
   pthread_create_possibly_lost
   Memcheck:Leak
   match-leak-kinds: possible
   ...
   fun:pthread_create*
}
{
   pthread_allocate_dtv
   Memcheck:Leak
   match-leak-kinds: possible
   fun:calloc
   ...
   fun:allocate_dtv
}
{
   pthread_allocate_stack
   Memcheck:Leak
   match-leak-kinds: possible
   ...
   fun:allocate_stack
}
{
   pthread_generic_possibly_lost
   Memcheck:Leak
   match-leak-kinds: possible
   fun:calloc
   ...
}
{
   pthread_arena
   Memcheck:Leak
   match-leak-kinds: possible
   ...
   fun:arena_get2
}
EOF
}

# Fonction pour afficher un titre
print_title() {
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Fonction pour tester avec timeout et détection mort
test_with_death() {
    local name=$1
    local args=$2
    local expected_death=$3

    echo -e "${YELLOW}Test: $name${NC}"
    echo -e "Commande: ./philo $args"
    output=$(timeout 5 ./philo $args 2>&1)
    if echo "$output" | grep -q "died"; then
        if [ "$expected_death" = "yes" ]; then
            echo -e "${GREEN}✓ PASS - Un philosophe est mort comme attendu${NC}\n"
        else
            echo -e "${RED}✗ FAIL - Un philosophe est mort alors qu'il ne devrait pas${NC}\n"
        fi
    else
        if [ "$expected_death" = "no" ]; then
            echo -e "${GREEN}✓ PASS - Aucun philosophe n'est mort${NC}\n"
        else
            echo -e "${RED}✗ FAIL - Aucun philosophe n'est mort alors qu'il devrait${NC}\n"
        fi
    fi
}

# Fonction pour tester avec limite de repas
test_with_meals() {
    local name=$1
    local args=$2

    echo -e "${YELLOW}Test: $name${NC}"
    echo -e "Commande: ./philo $args"
    output=$(timeout 10 ./philo $args 2>&1)
    if echo "$output" | grep -q "died"; then
        echo -e "${RED}✗ FAIL - Un philosophe est mort${NC}\n"
    else
        echo -e "${GREEN}✓ PASS - Programme terminé sans mort${NC}\n"
    fi
}

# Fonction pour test fuite mémoire (memcheck) - MODIFIÉE
run_valgrind_leak_test() {
    args=$1
    description=$2
    # Utiliser le fichier de suppression
    timeout 20 valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
      --suppressions=pthread.supp \
      --error-exitcode=99 ./philo $args > valgrind_out.log 2>&1

    if grep -q "definitely lost: 0 bytes" valgrind_out.log && \
       grep -q "indirectly lost: 0 bytes" valgrind_out.log && \
       grep -q "possibly lost: 0 bytes" valgrind_out.log; then
        echo -e "${GREEN}✓ LEAKFREE [$description] - $args${NC}\n"
    else
        echo -e "${RED}✗ LEAK DETECTED [$description] - ./philo $args${NC}"
        grep "lost:" valgrind_out.log | grep " bytes" | head -n 10
        echo ""
    fi
}

# Fonction pour test data race (hellgrind)
run_helgrind_test() {
    args=$1
    description=$2
    echo -e "${YELLOW}Hellgrind test: $description - args: $args${NC}"
    timeout 20 valgrind --tool=helgrind --error-exitcode=99 ./philo $args > helgrind_out.log 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Hellgrind passed: $description${NC}\n"
    else
        echo -e "${RED}✗ Hellgrind detected issues: $description${NC}"
        tail -n 20 helgrind_out.log
        echo ""
    fi
}

# ============================================
# CRÉER LE FICHIER DE SUPPRESSION AU DÉBUT
# ============================================
create_suppression_file

# Compilation
print_title "COMPILATION"
make re
if [ $? -ne 0 ]; then
    echo -e "${RED}Erreur de compilation${NC}"
    exit 1
fi
echo -e "${GREEN}Compilation réussie${NC}"
make clean

# Tests basiques
print_title "TESTS BASIQUES"
test_with_death "1 philosophe doit mourir" "1 800 200 200" "yes"
test_with_death "4 philosophes - ne doivent pas mourir" "4 410 200 200" "no"
test_with_death "5 philosophes - ne doivent pas mourir" "5 800 200 200" "no"

# Tests avec mort attendue
print_title "TESTS AVEC MORT ATTENDUE"
test_with_death "4 philosophes - mort attendue" "4 310 200 100" "yes"
test_with_death "3 philosophes - mort attendue" "3 310 200 100" "yes"

# Tests avec limite de repas
print_title "TESTS AVEC LIMITE DE REPAS"
test_with_meals "4 philosophes - 5 repas" "4 410 200 200 5"
test_with_meals "5 philosophes - 7 repas" "5 800 200 200 7"
test_with_meals "3 philosophes - 10 repas" "3 800 200 200 10"

# Tests nombres impairs
print_title "TESTS NOMBRES IMPAIRS"
test_with_death "3 philosophes - ne doivent pas mourir" "3 800 200 200" "no"
test_with_death "7 philosophes - ne doivent pas mourir" "7 800 200 200" "no"
test_with_death "9 philosophes - ne doivent pas mourir" "9 800 200 200" "no"

# Tests de stress
print_title "TESTS DE STRESS"
test_with_death "100 philosophes" "100 800 200 200" "no"
test_with_death "200 philosophes" "200 800 200 200" "no"

# Tests d'arguments invalides
print_title "TESTS ARGUMENTS INVALIDES"
echo -e "${YELLOW}Test: Arguments invalides${NC}"
./philo 4 -5 200 200 > /dev/null 2>&1
[ $? -eq 1 ] && echo -e "${GREEN}✓ PASS - Nombre négatif rejeté${NC}\n" || echo -e "${RED}✗ FAIL${NC}\n"
./philo 4 400.5 200 200 > /dev/null 2>&1
[ $? -eq 1 ] && echo -e "${GREEN}✓ PASS - Nombre décimal rejeté${NC}\n" || echo -e "${RED}✗ FAIL${NC}\n"
./philo 4 abc 200 200 > /dev/null 2>&1
[ $? -eq 1 ] && echo -e "${GREEN}✓ PASS - Lettres rejetées${NC}\n" || echo -e "${RED}✗ FAIL${NC}\n"

# Tests Leak Valgrind (memcheck)
print_title "VALGRIND MEMCHECK LEAKS TESTS"
run_valgrind_leak_test "1 800 200 200"           "Cas unique : 1 philosophe"
run_valgrind_leak_test "2 410 200 200"           "2 philosophes : cas minimal pair"
run_valgrind_leak_test "3 800 200 200"           "3 philosophes : cas impair basique"
run_valgrind_leak_test "4 410 200 200"           "4 philosophes stables"
run_valgrind_leak_test "5 800 200 200"           "5 philosophes stables"
run_valgrind_leak_test "5 310 200 100"           "5 philosophes avec mort attendue"
run_valgrind_leak_test "4 410 200 200 3"         "4 philosophes — 3 repas"
run_valgrind_leak_test "5 800 200 200 7"         "5 philosophes — 7 repas"
run_valgrind_leak_test "3 800 200 200 10"        "3 philosophes — 10 repas"
run_valgrind_leak_test "7 800 200 200"           "7 philosophes — 5 repas"
run_valgrind_leak_test "10 800 200 200"          "10 philosophes stress"
run_valgrind_leak_test "50 800 200 200"          "50 philosophes stress"
run_valgrind_leak_test "100 800 200 200"         "100 philosophes — gros stress test"
run_valgrind_leak_test "200 800 200 200"         "200 philosophes — test maximal"
run_valgrind_leak_test "4 400.5 200 200"         "Erreur d'arguments invalide (valide parsing)"
run_valgrind_leak_test "4 -100 200 200"          "Valeur négative refusée"
run_valgrind_leak_test "0 800 200 200"           "0 philosophes (doit quitter proprement)"

# Tests Hellgrind
print_title "VALGRIND HELLGRIND TESTS"
run_helgrind_test "1 800 200 200"           "1 philosophe simple"
run_helgrind_test "5 800 200 200 7"         "5 philosophes avec repas"
run_helgrind_test "7 800 200 200"           "7 philosophes stress"

# ============================================
# NETTOYAGE À LA FIN
# ============================================
rm -f pthread.supp valgrind_out.log helgrind_out.log

print_title "TESTS TERMINÉS"
echo -e "${GREEN}Tous les tests sont terminés !${NC}\n"
